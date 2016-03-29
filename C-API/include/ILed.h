#pragma once


#include <resultcodes.h>


namespace bigfish
{


class ILed
{
public:
    virtual RESULT write(bool on)   =0;
    virtual int get() const         =0;
};


} // namespace bigfish
