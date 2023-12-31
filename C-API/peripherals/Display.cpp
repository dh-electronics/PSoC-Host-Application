// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#include "Display.h"
#include "Compressor.h"
#include "peripherals/Gpios.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <c-api/bitmap.h>
#include <string.h>
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <new>


using namespace drc01;
using namespace std;


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


static void sigUserHandler(int)
{
    displayTakeScreenshot();
}


Display::Display(SpiProto &proto)
    : proto_(proto)
    , compressedLength_(0)
    , displayFilled_(true)
    , fillColorWhite_(false)
    , dumpToFile_(false)
    , screenshot_(false)
{
    // zeroing both buffers
    memset(buffer_, 0, BUFSIZE);
    memset(diff_, 0, BUFSIZE);

    // registering screenshot signal
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sigUserHandler;
    sa.sa_flags = 0;
    if(-1 == sigaction(SIGUSR1, &sa, NULL))
        fprintf(stderr, "Setting SIGUSR1 handler failed, %s", strerror(errno));
}


RESULT Display::enable(bool on)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);
    return proto_.gpioControl(RA4, on);
}


RESULT  Display::setContrast(int value)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    Command <1> cmd(CMD_DISPLAY_CONTRAST);
    Response <0> rsp;
    set_8(cmd.data(), 0, value);
    return proto_.xmit(cmd, rsp);
}


RESULT  Display::setDimTimeout(short value)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    Command <2> cmd(CMD_DISPLAY_DIM_TIMEOUT);
    Response <0> rsp;
    set_16(cmd.data(), 0, value);
    return proto_.xmit(cmd, rsp);
}


RESULT  Display::setOffTimeout(short value)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    Command <2> cmd(CMD_DISPLAY_OFF_TIMEOUT);
    Response <0> rsp;
    set_16(cmd.data(), 0, value);
    return proto_.xmit(cmd, rsp);
}

RESULT  Display::setOLedSaver(int enable)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    Command <1> cmd(CMD_DISPLAY_OLEDSAVER);
    Response <0> rsp;
    set_8(cmd.data(), 0, enable);
    return proto_.xmit(cmd, rsp);
}


void Display::fill(bool white)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    // remember that the display was filled with color
    displayFilled_ = true;
    fillColorWhite_ = white;
    compressedLength_ = 0;  // no compressed data makes sense anymore

    // fill both of the buffers with the color
    const uint8_t color = white ? 0xff : 0;
    memset(buffer_, color, BUFSIZE);
    memset(diff_, color, BUFSIZE);
}


void Display::fillRect(int x, int y, int w, int h, bool white)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    // check if the rect is outside the screen
    int xEnd, yEnd;
    if(!isRectOnScreen(x, y, w, h, xEnd, yEnd))
        return;

    // clamp to the screen
    const int xStart = x > 0 ? x : 0;
    if(xEnd > WIDTH)
        xEnd = WIDTH;           // non-inclusive

    const int yStart = y > 0 ? y : 0;
    if(yEnd > HEIGHT)
        yEnd = HEIGHT;      // non-inclusive
    const uint8_t rowWidth = xEnd - xStart;

    // prepare for drawing
    uint8_t yStartByte = yStart >> 3;       // divide by 8
    uint8_t yEndByte = yEnd >> 3;

    const uint8_t yStartBits = yStart & 0x07;
    const uint8_t yEndBits = yEnd & 0x07;   // xEndBits are inclusive

    uint8_t startMask = 0xFF << yStartBits;
    uint8_t endMask = ~(0xFF << yEndBits);

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

    uint8_t *d = bufferAddress(xStart, yStartByte);
    if(yStartByte < yEndByte)
    {
        // drawing the first column
        rowOp(d, startMask, rowWidth, op);
        d += WIDTH;

        // filling the intermediate block, if any
        uint8_t bytes = yEndByte - yStartByte;
        if(bytes > 1)
        {
            blockOp(d, white ? 0xff : 0, rowWidth, --bytes, bitwiseSet);
            d += bytes * WIDTH;
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
    rowOp(d, endMask, rowWidth, op);
}


void Display::drawRect(int x, int y, int w, int h, bool white)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

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
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    // check if the rect is outside the screen
    int xEnd, yEnd;
    if(!isRectOnScreen(x, y, w, h, xEnd, yEnd))
        return;

    // clamp to the screen
    const int xStart = x > 0 ? x : 0;
    if(xEnd > WIDTH)
        xEnd = WIDTH;       // non-inclusive

    const int yStart = y > 0 ? y : 0;
    if(yEnd > HEIGHT)
        yEnd = HEIGHT;      // non-inclusive

    const uint8_t rowWidth = xEnd - xStart;

    // prepare for drawing
    uint8_t yStartByte = yStart >> 3;       // divide by 8
    uint8_t yEndByte = yEnd >> 3;

    const uint8_t yStartBits = yStart & 0x07;
    const uint8_t yEndBits = yEnd & 0x07;   // xEndBits are inclusive

    uint8_t startMask = 0xFF << yStartBits;
    uint8_t endMask = ~(0xFF << yEndBits);
    uint8_t *d = bufferAddress(xStart, yStartByte);

    if(yStartByte < yEndByte)
    {
        // drawing the first column
        rowOp(d, startMask, rowWidth, bitwiseXor);
        d += WIDTH;

        // inverting the intermediate block, if any
        uint8_t bytes = yEndByte - yStartByte;
        if(bytes > 1)
        {
            blockOp(d, 0xff, rowWidth, --bytes, bitwiseXor);
            d += bytes * WIDTH;
        }
    }
    else
    { // start and end of x is on the same column
        endMask &= startMask;    // mask is for the black color
    }

    // drawing the last (or the only) column
    rowOp(d, endMask, rowWidth, bitwiseXor);
}


void Display::bitmap(int x, int y, const Bitmap &bmp)
{
    bitmap(x, y, bmp.width, bmp.height, bmp.pitch, bmp.data);
}


void Display::bitmap(int x, int y, const FT_Bitmap_ &bmp)
{
    if(bmp.pixel_mode == 1) // only PIXEL_MODE_MONO
        bitmap(x, y, bmp.width, bmp.rows, bmp.pitch, (uint8_t*)bmp.buffer);
}


RESULT Display::flush()
{
    if(dumpToFile_)
    {
        FILE * const f = fopen("dump.bdd", "wb");
        fwrite(buffer_, BUFSIZE, 1, f);
        fclose(f);
        dumpToFile_ = false;
    }

    if(screenshot_)
    {
        screenshot_ = false;
        writeBitmap();
    }

    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    {
        // create the diff
        uint8_t *b = buffer_;
        const uint8_t * const b_end = b + BUFSIZE;
        uint8_t *d = diff_;
        for(; b != b_end; ++b, ++d)
            *d ^= *b;
    }

    {
        // compress the diff buffer
        Compressor c(compressed_, diff_);
        compressedLength_ = c.compress();
    }

    // copy the data buffer to the diff
    memcpy(diff_, buffer_, BUFSIZE);

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

    const RESULT res = proto_.xmit(cmd, rsp, DISPLAY_FRAME_WAIT_MS);
    if(RESULT_OK != res)
        return res;

    return sendCompressed();
}


RESULT Display::swap()
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    // re-send the last compressed buffer
    if(compressedLength_)
    {
        // frame command
        Command <2> cmd(CMD_DISPLAY_FRAME);
        Response <0> rsp;
        set_16(cmd.data(), 0, compressedLength_);
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


RESULT Display::writeSplash()
{   
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    RESULT res = flush();
    if(RESULT_OK != res)
        return res;

    {   // erasing the flash
        Command <0> cmd(CMD_SPLASH_ERASE);
        Response <0> rsp;
        const RESULT res = proto_.xmit(cmd, rsp, SPLASH_ERASE_WAIT_MS);
        if(RESULT_OK != res)
            return res;
    }

    // transmitting the flash contents in the form of SPI packets with 12-byte data
    CommandUnion cu;
    cu.cmd.command_ = CMD_SPLASH_WRITE;
    uint8_t *dst = cu.cmd.data_;

    // computing the splash header
    dst[0] = uint8_t(compressedLength_);
    dst[1] = uint8_t((compressedLength_ >> 8) | 0x80 | (fillColorWhite_ ? 0x40 : 0));

    uint8_t offset = 2; // because 2 bytes are already in place
    const uint8_t *d = compressed_, * const d_end = compressed_ + compressedLength_;
    for(; d != d_end; ++d)
    {
        dst[offset] = *d;

        if(++offset == 12)
        {   // packet is full, transmit
            ResponseUnion ru;
            const RESULT res = proto_.xmit(&cu.cmd, offset + 2, &ru.rsp, 2, SPLASH_WRITE_WAIT_MS);
            if(RESULT_OK != res)
                return res;
            offset = 0;
        }
    }

    {   // transmitting the last packet (which can be a zero-size-data packet)
        ResponseUnion ru;
        const RESULT res = proto_.xmit(&cu.cmd, offset + 2, &ru.rsp, 2, SPLASH_WRITE_WAIT_MS);
        if(RESULT_OK != res)
            return res;
    }

    return RESULT_OK;
}


RESULT Display::screenshot()
{
    screenshot_ = true;
    return RESULT_OK;
}


void Display::restore()
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    displayFilled_ = true;      // to re-fill the display with the last defined color
    compressedLength_ = 0;      // no compressed data makes sense anymore

    // fill the diff buffer with the color
    const uint8_t color = fillColorWhite_ ? 0xff : 0;
    memset(diff_, color, BUFSIZE);

    flush();
}


uint8_t * Display::bufferAddress(uint8_t x, uint8_t byteY)
{
    return buffer_ + byteY * WIDTH + x;
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


void Display::rowOp(uint8_t *data, uint8_t val, uint8_t width, bitwiseOp *op)
{
    uint8_t *d = data;
    uint8_t *d_end = d + width;
    for(; d != d_end; ++d)
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
void Display::blockOp(uint8_t *data, uint8_t val, uint8_t width, uint8_t bytes, bitwiseOp *op)
{
    uint8_t *d = data;
    uint8_t * const d_end = d + bytes * WIDTH;
    for(; d !=  d_end; d += WIDTH)  // vertical loop
    {
        uint8_t *ld = d;
        const uint8_t *ld_end = ld + width;
        for(; ld != ld_end; ++ld)   // horizontal loop
            op(ld, val);
    }
}


void Display::horizontalLine(uint8_t xStart, uint8_t xEnd, uint8_t y, bool white)
{
    const uint8_t yByte = y >> 3;       // divide by 8
    uint8_t mask = 1 << (y & 0x07);
    const uint8_t width = xEnd - xStart + 1;

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

    uint8_t *d = bufferAddress(xStart, yByte);
    rowOp(d, mask, width, op);
}


void Display::verticalLine(uint8_t x, uint8_t yStart, uint8_t yEnd, bool white)
{
    // start of the line
    const uint8_t yStartByte = yStart >> 3;   // divide by 8
    const uint8_t yStartBit = yStart & 0x07;
    uint8_t startMask = 0xff << yStartBit;

    const uint8_t yEndByte = yEnd >> 3;       // divide by 8
    const uint8_t yEndBit = yEnd & 0x07;
    uint8_t endMask = ~(0xff << yEndBit);

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

    uint8_t *d = bufferAddress(x, yStartByte);
    if(yStartByte < yEndByte)
    {
        op(d, startMask);
        d += WIDTH;

        const uint8_t fillVal = white ? 0xff : 0;
        for(uint8_t yb = yStartByte + 1; yb < yEndByte; ++yb, d += WIDTH)
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


void Display::bitmap(int x, int y, int width, int height, int pitch, const uint8_t *data)
{
    std::lock_guard<std::recursive_mutex> lock(accessMutex_);

    int scrXEnd, scrYEnd;
    if(!isRectOnScreen(x, y, width, height, scrXEnd, scrYEnd))
        return;

    if(scrXEnd > WIDTH)
        scrXEnd = WIDTH;
    if(scrYEnd > HEIGHT)
        scrYEnd = HEIGHT;

    int scrX, bmpX;
    if(x < 0)
    {
        scrX = 0;
        bmpX = -x;
    }
    else
    {
        scrX = x;
        bmpX = 0;
    }

    int scrY, bmpY;
    if(y < 0)
    {
        scrY = 0;
        bmpY = -y;
    }
    else
    {
        scrY = y;
        bmpY = 0;
    }

    const uint8_t bitsInRow = scrXEnd - scrX;

    const int scrByte = scrY >> 3;
    int scrBit = scrY & 0x07;
    uint8_t *scr = bufferAddress(scrX, scrByte);

    const int bmpStartByte = bmpX >> 3;
    const int bmpStartBit = 7 - (bmpX & 0x07);
    const uint8_t *bmp = data + bmpStartByte + (pitch > 0 ? pitch * bmpY : -pitch * (height - bmpY - 1));

    for(int sy = scrY; sy < scrYEnd; ++sy)
    {   // start of the row
        const uint8_t *b = bmp;
        int8_t bmpBit = bmpStartBit;
        uint8_t *scrRow = scr;

        for(int bitsToDo = bitsInRow; bitsToDo; )
        {
            const uint8_t byte = *b;
            for(; bitsToDo && bmpBit >= 0; --bitsToDo, --bmpBit, ++scrRow)
                *scrRow ^= ((byte >> bmpBit) & 0x01) << scrBit;

            bmpBit = 7;
            ++b;
        }

        bmp += pitch;

        if(++scrBit > 7)
        {
            scrBit = 0;
            scr += WIDTH;
        }
    }

}


RESULT Display::sendCompressed()
{
    if(!compressedLength_)
        return RESULT_OK;

    CommandUnion cu;
    ResponseUnion ru;
    cu.cmd.command_ = CMD_DISPLAY_WRITE;

    const uint8_t MAX_DATA_BYTES = SPI_PACKET_SIZE - 2;
    const uint8_t *d = compressed_;
    uint16_t bytesSend;
    for(uint16_t bytesLeft = compressedLength_; bytesLeft > 0; bytesLeft -= bytesSend, d += bytesSend)
    {
        bytesSend = bytesLeft > MAX_DATA_BYTES ? MAX_DATA_BYTES : bytesLeft;
        memcpy(cu.cmd.data_, d, bytesSend);

        const RESULT res = proto_.xmit(&cu.cmd, bytesSend + 2, &ru.rsp, 2, DISPLAY_DATA_WAIT_MS);
        if(RESULT_OK != res)
            return res;
    }

    return RESULT_OK;
}


void Display::writeBitmap()
{

#pragma pack(push, 1)
    struct BMPFILEHEADER
    {
        uint16_t    type;       //specifies the file type
        uint32_t    size;       //specifies the size in bytes of the bitmap file
        uint16_t    reserved1;  //reserved; must be 0
        uint16_t    reserved2;  //reserved; must be 0
        uint32_t    offBits;    //species the offset in bytes from the bitmapfileheader to the bitmap bits
    };

    struct BMPINFOHEADER
    {
        uint32_t    size;       //specifies the number of bytes required by the struct
        int32_t     width;      //specifies width in pixels
        int32_t     height;     //species height in pixels
        uint16_t    planes;     //specifies the number of color planes, must be 1
        uint16_t    bitCount;   //specifies the number of bit per pixel
        uint32_t    compression;//spcifies the type of compression
        uint32_t    sizeImage;  //size of image in bytes
        int32_t     XPelsPerMeter;  //number of pixels per meter in x axis
        int32_t     YPelsPerMeter;  //number of pixels per meter in y axis
        uint32_t    clrUsed;        //number of colors used by the palette
        uint32_t    clrImportant;   //number of colors that are important
    };
#pragma pack(pop)

    // find a free file name
    FILE *f;
    {
        char filename[32];
        for(int8_t i = 1; i <= 16; ++i)
        {
            sprintf(filename, "screenshot_%hhd.bmp", i);
            if(access(filename, F_OK) == -1)
                break;
        }

        // open a new file
        f = fopen(filename, "wb");
        if(f == NULL)
        {
            fprintf(stderr, "Could not cretae screenshot file.");
            return;
        }
    }


    {
        // write the header
        uint8_t colortable[] = {0, 0, 0, 0, 0xff, 0xff, 0xff, 0};

        const uint32_t offset = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(colortable);
        const BMPFILEHEADER bmpHeader = {
            0x4d42, // header 'BM'
            offset + BUFSIZE,    // file size
            0, 0,   // reserved
            offset
        };
        fwrite(&bmpHeader, sizeof(bmpHeader), 1, f);

        // write the info
        const BMPINFOHEADER infoHeader = {
            sizeof(BMPINFOHEADER),
            128, 64,            // width, height
            1,                  // 1 color plane
            1,                  // 1 bit per pixel
            0,                  // no compression
            BUFSIZE,            // image size in bytes
            128*1000/28, 64*1000/14, // pixels per meter for x, y
            2, 2                // number of colors palette/important

        };
        fwrite(&infoHeader, sizeof(infoHeader), 1, f);

        // write the color table
        fwrite(colortable, sizeof(colortable), 1, f);
    }

    { // recode
        uint8_t dst_buffer[BUFSIZE];
        memset(dst_buffer, 0, sizeof(dst_buffer));

        const uint16_t width_mask = WIDTH - 1;
        const uint16_t row_bytes = WIDTH >> 3;
        for(uint16_t i = 0; i < BUFSIZE; ++i)
        {
            const uint16_t offset =  BUFSIZE - row_bytes - (i & ~width_mask) + ((i & width_mask) >> 3);
            uint8_t *dst = dst_buffer + offset;
            const uint8_t dst_mask = 0x80 >> (i & 7);
            uint8_t val = buffer_[i];
            for(int8_t b = 0; b < 8; ++b, dst -= row_bytes, val >>= 1)
            {
                if(val & 1)
                    *dst |= dst_mask;
            }
        }
        fwrite(dst_buffer, sizeof(dst_buffer), 1, f);
    }

    fclose(f);
}

