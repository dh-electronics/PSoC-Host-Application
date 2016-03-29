#pragma once


enum RESULT;


namespace bigfish
{


class IDisplay
{
public:
    virtual RESULT enable(bool on)              =0;
    virtual RESULT clear()                      =0;
    virtual RESULT invert(bool on)              =0;
    virtual RESULT selectFont(int font)         =0;
    virtual RESULT setPosition(int x, int y)    =0;
    virtual RESULT text(const char *text)       =0;
    virtual RESULT show()                       =0;
};


} // namespace bigfish
