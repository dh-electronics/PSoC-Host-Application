#pragma once


#include "display.h"
#include <peripherals/IDisplay.h>
#include <stdint.h>


namespace drc01
{


class SpiProto;


class Display : public IDisplay
{
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 64;

    static const uint16_t DISPLAY_CLEAR_WAIT_MS    = 2;
    static const uint16_t DISPLAY_SYMBOL_WAIT_MS   = 1;
    static const uint16_t DISPLAY_SHOW_WAIT_MS     = 12; // 9, 10

    Display(SpiProto &proto);

    virtual RESULT  enable(bool on);
    virtual void    fill(bool white);
    virtual void    fillRect(int x, int y, int w, int h, bool white);
    virtual void    drawRect(int x, int y, int w, int h, bool white);
    virtual void    invertRect(int x, int y, int w, int h);

    virtual void    bitmap(int x, int y, const Bitmap &bmp);
    virtual void    bitmap(int x, int y, const FT_Bitmap &bmp);

    virtual RESULT flush();
    virtual RESULT swap();

private:
    static const int PITCH = WIDTH / 8;
    static const int BUFSIZE = PITCH * HEIGHT;

    uint8_t *   bufferAddress(uint8_t byteX, uint8_t y);
    static bool isRectOnScreen(int x, int y, int w, int h, int &xEnd, int &yEnd);
    static void columnOr(uint8_t *data, uint8_t mask, uint8_t height);
    static void columnXor(uint8_t *data, uint8_t mask, uint8_t height);
    static void columnAnd(uint8_t *data, uint8_t mask, uint8_t height);
    static void columnSet(uint8_t *data, uint8_t mask, uint8_t height);
    void horizontalLine(uint8_t xStart, uint8_t xEnd, uint8_t y);
    void verticalLine(uint8_t x, uint8_t yStart, uint8_t yEnd);

    SpiProto &  proto_;
    uint8_t     buffer_[BUFSIZE];               // main buffer
    uint8_t     diffBuffer_[BUFSIZE];           // buffer used for diff
    uint8_t     compressedDiff_[BUFSIZE + 3];   // compressed diff of the two buffers
    uint16_t    compressedSize_;
    uint16_t    sentByteIdx_;
    bool        displayFilled_;
    bool        fillColorWhite_;
};


}
