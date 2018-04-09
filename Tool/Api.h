#pragma once


#include "Service.h"
#include <stdint.h>


class Api : public Service
{
public:
    static bool parseArgs(int argc, char **argv, uint16_t &idx);
    static bool mustRemain() { return remain_; }

private:
    static bool remain_;
};
