#include "Display.h"
#include <display.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


bool Display::parseArgs(int argc, char **argv, uint16_t &idx)
{
    if(0 != strcmp(argv[idx], "display"))
        return false;

    ++idx;
    while(idx < argc)
    {
        if(0 == strcmp(argv[idx], "enable"))
        {
            ++idx;
            displayEnable(true);
            displaySetContrast(0xff);
        }
        else if(0 == strcmp(argv[idx], "disable"))
        {
            ++idx;
            displayEnable(false);
        }
        else if(0 == strcmp(argv[idx], "fill"))
        {
            ++idx;
            displayFill(getInt(argc, argv, idx, 0));
        }
        else if(0 == strcmp(argv[idx], "fillrect"))
        {
            ++idx;
            displayFillRect(
                        getInt(argc, argv, idx, 60),
                        getInt(argc, argv, idx, 28),
                        getInt(argc, argv, idx, 8),
                        getInt(argc, argv, idx, 8),
                        getInt(argc, argv, idx, 1));
        }
        else if(0 == strcmp(argv[idx], "show"))
        {
            ++idx;
            displayFlush();
        }
        else
        {
            break;
        }
    }
    return true;
}
