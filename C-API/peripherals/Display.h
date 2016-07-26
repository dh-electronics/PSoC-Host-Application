#pragma once


#include "display.h"
#include <peripherals/IDisplay.h>
#include <Poco/Mutex.h>
#include <stdint.h>


namespace drc01
{


class SpiProto;


class Display : public IDisplay
{
public:
    static const int WIDTH = 128;
    static const int HEIGHT = 64;
    static const int BUFSIZE = WIDTH * HEIGHT / 8;

    static const uint16_t DISPLAY_FRAME_WAIT_MS     = 2;
    static const uint16_t DISPLAY_DATA_WAIT_MS      = 3;

    Display(SpiProto &proto);

    virtual RESULT  enable(bool on);
    virtual void    fill(bool white);
    virtual void    fillRect(int x, int y, int w, int h, bool white);
    virtual void    drawRect(int x, int y, int w, int h, bool white);
    virtual void    invertRect(int x, int y, int w, int h);

    virtual void    bitmap(int x, int y, const Bitmap &bmp);
    virtual void    bitmap(int x, int y, const FT_Bitmap_ &bmp);

    virtual RESULT flush();
    virtual RESULT swap();

private:
    uint8_t *   bufferAddress(uint8_t x, uint8_t byteY);
    static bool isRectOnScreen(int x, int y, int w, int h, int &xEnd, int &yEnd);

    typedef void bitwiseOp(uint8_t *, uint8_t);
    static void rowOp(uint8_t *data, uint8_t mask, uint8_t width, bitwiseOp *op);
    static void blockOp(uint8_t *data, uint8_t val, uint8_t width, uint8_t bytes, bitwiseOp *op);

    void        horizontalLine(uint8_t xStart, uint8_t xEnd, uint8_t y, bool white);
    void        verticalLine(uint8_t x, uint8_t yStart, uint8_t yEnd, bool white);
    void        bitmap(int x, int y, int width, int height, int pitch, const uint8_t *data);

    RESULT      sendCompressed();

    SpiProto &  proto_;
    uint8_t     buffer_[BUFSIZE];               // main buffer
    uint8_t     diff_[BUFSIZE];                 // buffer used for diff
    uint8_t     compressed_[BUFSIZE + 3];       // compressed diff of the two buffers
    uint16_t    compressedLength_;
    bool        displayFilled_;
    bool        fillColorWhite_;

    mutable Poco::FastMutex accessMutex_;
};


}
