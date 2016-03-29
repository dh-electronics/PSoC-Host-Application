#pragma once


#include "display.h"
#include <IDisplay.h>
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
    virtual RESULT clear();
    virtual RESULT invert(bool on);
    virtual RESULT selectFont(int font);
    virtual RESULT setPosition(int x, int y);
    virtual RESULT text(const char *text);
    virtual RESULT show();

private:
    SpiProto & proto_;
};


} // namespace bigfish
