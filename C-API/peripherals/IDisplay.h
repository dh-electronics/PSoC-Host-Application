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

    virtual RESULT enable(bool on)                                                  =0;
    virtual RESULT fill(bool white)                                                 =0;
    virtual RESULT fillRect(int x, int y, int w, int h, bool white)                 =0;
    virtual RESULT drawRect(int x, int y, int w, int h, bool white)                 =0;
    virtual RESULT invertRect(int x, int y, int w, int h)                           =0;

    virtual RESULT bitmap(int x, int y, const Bitmap &bmp)                          =0;
    virtual RESULT bitmap(int x, int y, const FT_Bitmap &bmp)                       =0;

    virtual RESULT flush()      =0;
    virtual RESULT swap()       =0;
};


}
