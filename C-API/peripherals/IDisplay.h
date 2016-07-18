#pragma once


#include "display.h"


enum RESULT;


struct Bitmap;
struct FT_Bitmap;


namespace drc01
{


class IDisplay
{
public:
    virtual ~IDisplay() {}

    virtual RESULT  enable(bool on)                                                  =0;
    virtual void    fill(bool white)                                                 =0;
    virtual void    fillRect(int x, int y, int w, int h, bool white)                 =0;
    virtual void    drawRect(int x, int y, int w, int h, bool white)                 =0;
    virtual void    invertRect(int x, int y, int w, int h)                           =0;

    virtual void    bitmap(int x, int y, const Bitmap &bmp)                          =0;
    virtual void    bitmap(int x, int y, const FT_Bitmap &bmp)                       =0;

    virtual RESULT flush()      =0;
    virtual RESULT swap()       =0;
};


}
