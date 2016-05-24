#pragma once


#include "display.h"


enum RESULT;


namespace drc01
{


class IDisplay
{
public:
    virtual ~IDisplay() {}

    virtual RESULT enable(bool on)              =0;
    virtual RESULT clear()                      =0;
    virtual RESULT invert(bool on)              =0;
    virtual RESULT setPosition(int x, int y)    =0;
    virtual RESULT bitmap(const unsigned char *data, int width, int height, BMP_FUNC func)   =0;
};


} // namespace bigfish
