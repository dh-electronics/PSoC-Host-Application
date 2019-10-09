#pragma once


#include "Service.h"
#include <c-api/buttons.h>
#include <stdint.h>


class Buttons : public Service
{
public:
    static bool parseArgs(int argc, char **argv, uint16_t &idx);

private:
    static void callback(BUTTON button, bool pressed);
};
