#pragma once


#include "peripherals/Led.h"
#include "peripherals/Gpios.h"
#include <stdint.h>


namespace drc01
{


class SpiProto;


class LedOnGpio : public Led
{
public:
    static const int COUNT   = 3;

    LedOnGpio(SpiProto &proto, GPIO gpio);

    virtual RESULT write(bool on);

private:
    SpiProto & proto_;
    const GPIO gpio_;
};


} // namespace bigfish
