#pragma once


#include "Service.h"
#include <stdint.h>


class Leds : public Service
{
public:
    static bool parseArgs(int argc, char **argv, uint16_t &idx);
};
