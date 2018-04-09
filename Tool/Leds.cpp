#include "Leds.h"
#include <leds.h>
#include <string.h>


bool Leds::parseArgs(int argc, char **argv, uint16_t &idx)
{
    if(0 != strcmp(argv[idx], "leds"))
        return false;

    ++idx;
    while(idx < argc)
    {
        static const char *strLeds[] = {"RUN","LAN","BUS","ERR"};
        int ledIdx;
        for(ledIdx = 0; ledIdx <= LED_ERR; ++ledIdx)
        {
            if(0 == strcmp(argv[idx], strLeds[ledIdx]))
            {
                ++idx;
                break;
            }
        }
        if(ledIdx > LED_ERR)
            break;

        writeLed(LED(ledIdx), 0 != getInt(argc, argv, idx, 1));
    }
    return true;
}
