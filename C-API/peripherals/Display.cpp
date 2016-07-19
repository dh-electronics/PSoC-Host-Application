#include "Display.h"
#include "Compressor.h"
#include "peripherals/Gpios.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <Poco/ScopedLock.h>
#include <string.h>
#include <cassert>


using namespace drc01;
using namespace std;
using namespace Poco;


Display::Display(SpiProto &proto)
    : proto_(proto)
    , compressedLength_(0)
    , displayFilled_(true)
    , fillColorWhite_(false)
{
    // zeroing both buffers
    memset(buffer_, 0, BUFSIZE);
    memset(diffBuffer_, 0, BUFSIZE);
}


RESULT Display::enable(bool on)
{
    return proto_.gpioControl(RA4, on);
}


void Display::fill(bool white)
{
    // remember that the display was filled with color
    displayFilled_ = true;
    fillColorWhite_ = white;
    compressedLength_ = 0;  // no compressed data makes sense anymore

    // fill both of the buffers with the color
    const uint8_t color = white ? 0xff : 0;
    memset(buffer_, color, BUFSIZE);
    memset(diffBuffer_, color, BUFSIZE);
}


uint8_t * Display::bufferAddress(uint8_t byteX, uint8_t y)
{
    return buffer_ + byteX + PITCH * y;
}


/**
 * @brief isRectOnScreen
 * @param x
 * @param y
 * @param w - width of the rectangle, min value is 1
 * @param h - height of the rectangle, min value is 1
 * @param xEnd - returned end of x, non-inclusive
 * @param yEnd - returned end of y, non-inclusive
 * @return - true means rect intersects with the screen
 */
bool Display::isRectOnScreen(int x, int y, int w, int h, int &xEnd, int &yEnd)
{
    if(w < 1 || h < 1)
        return false;

    if(y >= HEIGHT || x >= WIDTH)
        return false;

    xEnd = x + w;
    if(xEnd <= 0)
        return false;

    yEnd = y + h;
    if(yEnd <= 0)
        return false;

    return true;
}


static void bitwiseOr(uint8_t *res, uint8_t arg)
{
    *res |= arg;
}


static void bitwiseAnd(uint8_t *res, uint8_t arg)
{
    *res &= arg;
}


static void bitwiseXor(uint8_t *res, uint8_t arg)
{
    *res ^= arg;
}


static void bitwiseSet(uint8_t *res, uint8_t arg)
{
    *res = arg;
}


void Display::columnOp(uint8_t *data, uint8_t val, uint8_t height, bitwiseOp *op)
{
    uint8_t *d = data;
    uint8_t *d_end = d + PITCH * height;
    for(; d != d_end; d += PITCH)
        op(d, val);
}


/**
 * @brief Display::blockOp fast block-filling function, optimized for CPU cache
 * @param data byte on the left-top
 * @param val - value to use
 * @param bytes - how many bytes horizontally
 * @param height - vertical height
 * @param op
 */
void Display::blockOp(uint8_t *data, uint8_t val, uint8_t bytes, uint8_t height, bitwiseOp *op)
{
    uint8_t *d = data;
    for(uint8_t i = 0; i < height; ++i, d += PITCH)
    {
        uint8_t *ld = d;
        const uint8_t *ld_end = ld + bytes;
        for(; ld != ld_end; ++ld)
            op(ld, val);
    }
}


void Display::horizontalLine(uint8_t xStart, uint8_t xEnd, uint8_t y, bool white)
{
    // start of the line
    const uint8_t xStartByte = xStart >> 3;       // divide by 8
    const uint8_t xStartBit = xStart & 0x07;
    uint8_t startMask = 1 << xStartBit;

    const uint8_t xEndByte = xEnd >> 3;       // divide by 8
    const uint8_t xEndBit = xEnd & 0x07;
    uint8_t endMask = 1 << xEndBit;

    bitwiseOp *op;
    if(white)
    {
        op = bitwiseOr;
    }
    else
    {
        op = bitwiseAnd;
        startMask = ~startMask;
        endMask = ~endMask;
    }

    uint8_t *d = bufferAddress(xStartByte, y);
    if(xStartByte < xEndByte)
    {
        op(d, startMask);
        ++d;

        const uint8_t fillVal = white ? 0xff : 0;
        for(uint8_t xb = xStartByte + 1; xb < xEndByte; ++xb, ++d)
            *d = fillVal;
    }
    else if(white)
    {
        endMask &= startMask;
    }
    else
    {
        endMask |= startMask;
    }

    op(d, endMask);
}


void Display::verticalLine(uint8_t x, uint8_t yStart, uint8_t yEnd, bool white)
{
    const uint8_t xByte = x >> 3;       // divide by 8
    uint8_t mask = 1 << (x & 0x07);
    const uint8_t height = yEnd - yStart + 1;

    bitwiseOp *op;
    if(white)
    {
        op = bitwiseOr;
    }
    else
    {
        op = bitwiseAnd;
        mask = ~mask;
    }

    uint8_t *d = bufferAddress(xByte, yStart);
    columnOp(d, mask, height, op);
}


void Display::fillRect(int x, int y, int w, int h, bool white)
{
    // check if the rect is outside the screen
    int xEnd, yEnd;
    if(!isRectOnScreen(x, y, w, h, xEnd, yEnd))
        return;

    --xEnd; // to inclusive

    // clamp to the screen
    const int xStart = x > 0 ? x : 0;
    if(xEnd >= WIDTH)
        xEnd = WIDTH - 1;   // inclusive

    const int yStart = y > 0 ? y : 0;
    if(yEnd > HEIGHT)
        yEnd = HEIGHT;      // non-inclusive
    const uint8_t columnHeight = yEnd - yStart;

    // prepare for drawing
    uint8_t xStartByte = xStart >> 3;       // divide by 8
    uint8_t xEndByte = xEnd >> 3;

    const uint8_t xStartBits = xStart & 0x07;
    const uint8_t xEndBits = xEnd & 0x07;   // xEndBits are inclusive

    uint8_t startMask = 0xFF << xStartBits;
    uint8_t endMask = ~(0xFF << xEndBits);

    bitwiseOp *op;
    if(white)
    {
        op = bitwiseOr;
    }
    else
    {
        op = bitwiseAnd;
        startMask = ~startMask;
        endMask = ~endMask;
    }

    uint8_t *d = bufferAddress(xStartByte, yStart);
    if(xStartByte < xEndByte)
    {
        // drawing the first column
        columnOp(d, startMask, columnHeight, op);
        ++d;

        // filling the intermediate block, if any
        uint8_t bytes = xEndByte - xStartByte;
        if(bytes > 1)
        {
            blockOp(d, white ? 0xff : 0, --bytes, columnHeight, bitwiseSet);
            d += bytes;
        }
    }
    else if(white) // start and end of x is on the same column
    {
        endMask &= startMask;
    }
    else
    {
        endMask |= startMask;
    }

    // drawing the last (or the only) column
    columnOp(d, endMask, columnHeight, op);
}


void Display::drawRect(int x, int y, int w, int h, bool white)
{
    // check if the rect is outside the screen
    int xEnd, yEnd;
    if(!isRectOnScreen(x, y, w, h, xEnd, yEnd))
        return;

    --xEnd; --yEnd; // to include the endpoints

    // clamp to the screen
    bool drawLeft;
    int xStart;
    if(x >= 0)
    {
        xStart = x;
        drawLeft = true;
    }
    else
    {
        xStart = 0;
        drawLeft = false;
    }

    bool drawRight;
    if(xEnd < WIDTH)
    {
        drawRight = true;
    }
    else
    {
        xEnd = WIDTH - 1;   // inclusive
        drawRight = false;
    }

    int yStart;
    bool drawTop;
    if(y >= 0)
    {
       yStart = y;
       drawTop = true;
    }
    else
    {
        yStart = 0;
        drawTop = false;
    }

    bool drawBottom;
    if(yEnd < HEIGHT)
    {
        drawBottom = true;
    }
    else
    {
        yEnd = HEIGHT - 1;
        drawBottom = false;
    }

    // drawing
    if(drawTop)
        horizontalLine(xStart, xEnd, yStart, white);

    if(drawBottom)
        horizontalLine(xStart, xEnd, yEnd, white);

    if(drawLeft)
        verticalLine(xStart, yStart, yEnd, white);

    if(drawRight)
        verticalLine(xEnd, yStart, yEnd, white);
}


void Display::invertRect(int x, int y, int w, int h)
{
    // check if the rect is outside the screen
    int xEnd, yEnd;
    if(!isRectOnScreen(x, y, w, h, xEnd, yEnd))
        return;

    --xEnd; // to inclusive

    // clamp to the screen
    const int xStart = x > 0 ? x : 0;
    if(xEnd >= WIDTH)
        xEnd = WIDTH - 1;   // inclusive

    const int yStart = y > 0 ? y : 0;
    if(yEnd > HEIGHT)
        yEnd = HEIGHT;      // non-inclusive

    const uint8_t columnHeight = yEnd - yStart;

    // prepare for drawing
    uint8_t xStartByte = xStart >> 3;       // divide by 8
    uint8_t xEndByte = xEnd >> 3;

    const uint8_t xStartBits = xStart & 0x07;
    const uint8_t xEndBits = xEnd & 0x07;   // xEndBits are inclusive

    uint8_t startMask = 0xFF << xStartBits;
    uint8_t endMask = ~(0xFF << xEndBits);
    uint8_t *d = bufferAddress(xStartByte, yStart);

    if(xStartByte < xEndByte)
    {
        // drawing the first column
        columnOp(d, startMask, columnHeight, bitwiseXor);

        ++d;

        // inverting the intermediate block, if any
        uint8_t bytes = xEndByte - xStartByte;
        if(bytes > 1)
        {
            blockOp(d, 0xff, --bytes, columnHeight, bitwiseXor);
            d += bytes;
        }
    }
    else
    { // start and end of x is on the same column
        endMask &= startMask;    // mask is for the black color
    }

    // drawing the last (or the only) column
    columnOp(d, endMask, columnHeight, bitwiseXor);
}


void Display::bitmap(int x, int y, const Bitmap &bmp)
{
    // TODO:
}


void Display::bitmap(int x, int y, const FT_Bitmap &bmp)
{
    // TODO:
}


RESULT Display::flush()
{
    // create a diff
    uint8_t *b = buffer_;
    uint8_t *b_end = b + BUFSIZE;
    uint8_t *d = diffBuffer_;
    for(; b != b_end; ++b, ++d)
        *d ^= *b;

    // compress the diff buffer and send
    {
        Compressor c(compressed_, diffBuffer_);
        compressedLength_ = c.compress();
    }

    // copy the buffer
    memcpy(diffBuffer_, buffer_, BUFSIZE);

    // frame command
    Command <2> cmd(CMD_DISPLAY_FRAME);
    Response <0> rsp;
    uint16_t params = compressedLength_;
    if(displayFilled_)
    {
        displayFilled_ = false;
        params |= 0x8000;           // fill flag
        if(fillColorWhite_)
            params |= 0x4000;       // color white
    }
    set_16(cmd.data(), 0, params);

    ScopedLock <FastMutex> lock(accessMutex_);
    const RESULT res = proto_.xmit(cmd, rsp, DISPLAY_FRAME_WAIT_MS);
    if(RESULT_OK != res)
        return res;

    return sendCompressed();
}


RESULT Display::swap()
{
    // re-send the last compressed buffer
    if(compressedLength_)
    {
        // frame command
        Command <2> cmd(CMD_DISPLAY_FRAME);
        Response <0> rsp;
        set_16(cmd.data(), 0, compressedLength_);
        ScopedLock <FastMutex> lock(accessMutex_);
        const RESULT res = proto_.xmit(cmd, rsp, DISPLAY_FRAME_WAIT_MS);
        if(RESULT_OK != res)
            return res;

        return sendCompressed();
    }
    else
    {
        return RESULT_OK;
    }
}


RESULT Display::sendCompressed()
{
    uint8_t *d = compressed_;


    CommandUnion cu;
    ResponseUnion ru;
    cu.cmd.command_ = CMD_DISPLAY_DATA;

    uint16_t bytesSend;
    for(uint16_t bytesLeft = compressedLength_; bytesLeft > 0; bytesLeft -= bytesSend, d += bytesSend)
    {
        bytesSend = bytesLeft > SPI_PACKET_SIZE ? SPI_PACKET_SIZE : bytesLeft;
        memcpy(cu.cmd.data_, d, bytesSend);

        const RESULT res = proto_.xmit(&cu.cmd, bytesSend + 2, &ru.rsp, 2, DISPLAY_DATA_WAIT_MS);
        if(RESULT_OK != res)
            return res;
    }

    return RESULT_OK;
}
