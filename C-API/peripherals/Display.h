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
    static const uint16_t DISPLAY_CLEAR_WAIT_MS    = 2;
    static const uint16_t DISPLAY_SYMBOL_WAIT_MS   = 1;
    static const uint16_t DISPLAY_SHOW_WAIT_MS     = 12; // 9, 10

    Display(SpiProto &proto);

    virtual RESULT enable(bool on);
    virtual RESULT fill(bool white);
    virtual RESULT fillRect(int x, int y, int w, int h, bool white);
    virtual RESULT drawRect(int x, int y, int w, int h, bool white);
    virtual RESULT invertRect(int x, int y, int w, int h);

    virtual RESULT bitmap(int x, int y, const Bitmap &bmp);
    virtual RESULT bitmap(int x, int y, const FT_Bitmap &bmp);

    virtual RESULT flush();
    virtual RESULT swap();

private:
    static const int WIDTH = 128;
    static const int HEIGHT = 64;
    static const int BUFSIZE = WIDTH * HEIGHT / 8;

    SpiProto & proto_;
    uint8_t     buffers_[2][BUFSIZE];           // two buffers
    uint8_t     compressedDiff_[BUFSIZE + 3];   // compressed diff
    uint16_t    compressedSize_;
    uint16_t    sentByteIdx_;
    uint8_t     currentBufferIdx_;              // current buffer index
    uint8_t *   currentBuffer_;                // pointer to current buffer
};


}
