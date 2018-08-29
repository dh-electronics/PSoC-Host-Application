#pragma once


#include <resultcodes.h>


namespace drc01
{


class ILed
{
public:
    virtual ~ILed() {}
    virtual RESULT write(bool on)   =0;
    virtual int get() const         =0;
};


}
