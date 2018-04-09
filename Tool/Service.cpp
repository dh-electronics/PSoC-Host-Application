#include "Service.h"
#include <stdio.h>


int Service::getInt(int argc, char **argv, uint16_t &idx, int defaultVal)
{
    if(idx >= argc)
        return defaultVal;

    int val;
    if(sscanf(argv[idx], "%d", &val) == 1)
    {
        ++idx;
        return  val;
    }
    else
    {
        return  defaultVal;
    }
}
