#pragma once

#include <stdint.h>

class Service
{
protected:
    static int getInt(int argc, char **argv, uint16_t &idx, int defaultVal);
};
