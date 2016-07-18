#include "Display.h"
#include "peripherals/Gpios.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <string.h>
#include <cassert>


using namespace drc01;
using namespace std;


Display::Display(SpiProto &proto)
    : proto_(proto)
    , displayFilled_(false)
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


void Display::columnOr(uint8_t *data, uint8_t mask, uint8_t height)
{
    uint8_t *d = data;
    for(uint8_t i = 0; i < height; ++i, d += Display::PITCH)
        *d |= mask;
}


void Display::columnXor(uint8_t *data, uint8_t mask, uint8_t height)
{
    uint8_t *d = data;
    for(uint8_t i = 0; i < height; ++i, d += Display::PITCH)
        *d ^= mask;
}


void Display::columnAnd(uint8_t *data, uint8_t mask, uint8_t height)
{
    uint8_t *d = data;
    for(uint8_t i = 0; i < height; ++i, d += Display::PITCH)
        *d &= mask;
}


void Display::columnSet(uint8_t *data, uint8_t mask, uint8_t height)
{
    uint8_t *d = data;
    for(uint8_t i = 0; i < height; ++i, d += Display::PITCH)
        *d = mask;
}


void Display::horizontalLine(uint8_t xStart, uint8_t xEnd, uint8_t y)
{
    // start of the line
    const uint8_t xStartByte = x >> 3;       // divide by 8
    const uint8_t bit = x & 0x07;
    const uint8_t mask = 1 << bit;
}


void Display::verticalLine(uint8_t x, uint8_t yStart, uint8_t yEnd)
{
    const uint8_t xByte = x >> 3;       // divide by 8
    const uint8_t mask = 1 << (x & 0x07);
    const uint8_t height = yEnd - yStart + 1;

    uint8_t *d = bufferAddress(xByte, yStart);
    columnOr(d, mask, height);
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
    const uint8_t columnHeight = yEnd - yStart;

    const int yStart = y > 0 ? y : 0;
    if(yEnd > HEIGHT)
        yEnd = HEIGHT;      // non-inclusive

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
        if(white)
            columnOr(d, startMask, columnHeight);
        else
            columnAnd(d, ~startMask, columnHeight);

        ++d;

        // filling the intermediate columns
        for(uint8_t xb = xStartByte + 1; xb < xEndByte; ++xb, ++d)
            columnSet(d, white ? 0xff : 0, columnHeight);
    }
    else
    { // start and end of x is on the same column
        endMask &= startMask;    // mask is for the black color
    }

    // drawing the last (or the only) column
    if(white)
        columnOr(d, endMask, columnHeight);
    else
        columnAnd(d, ~endMask, columnHeight);
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

    const uint8_t columnHeight = yEnd - yStart;

    const int yStart;
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
        horizontalLine(xStart, xEnd, yStart);

    if(drawBottom)
        horizontalLine(xStart, xEnd, yEnd);

    if(drawLeft)
        verticalLine(xStart, yStart, yEnd);

    if(drawRight)
        verticalLine(xEnd, yStart, yEnd);
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
    const uint8_t columnHeight = yEnd - yStart;

    const int yStart = y > 0 ? y : 0;
    if(yEnd > HEIGHT)
        yEnd = HEIGHT;      // non-inclusive

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
        columnXor(d, startMask, columnHeight);

        ++d;

        // filling the intermediate columns
        for(uint8_t xb = xStartByte + 1; xb < xEndByte; ++xb, ++d)
            columnXor(d, 0xff, columnHeight);
    }
    else
    { // start and end of x is on the same column
        endMask &= startMask;    // mask is for the black color
    }

    // drawing the last (or the only) column
    columnXor(d, endMask, columnHeight);
}


void Display::bitmap(int x, int y, const Bitmap &bmp)
{
}


void Display::bitmap(int x, int y, const FT_Bitmap &bmp)
{
}


RESULT Display::flush()
{


    if(displayFilled_)
    {

        displayFilled_ = false;
    }
    return RESULT_OK;
}


RESULT Display::swap()
{
    return RESULT_OK;
}
