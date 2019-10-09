#include "Api.h"
#include <c-api/init.h>
#include <string.h>
#include <stdio.h>
#include <c-api/buttons.h>
#include <thread>
#include <chrono>


extern bool running;
bool Api::remain_ = false;


bool Api::parseArgs(int, char **argv, uint16_t &idx)
{
    if(0 == strcmp(argv[idx], "idle"))
    {
        ++idx;
        idleApi();
        return true;
    }
    else if(0 == strcmp(argv[idx], "loop"))
    {
        ++idx;
        while(running)
        {
            handleButtons();
            std::this_thread::sleep_for(std::chrono::milliseconds{50});

        }
        setButtonsCallback(NULL);
        return true;
    }
    else if(0 == strcmp(argv[idx], "keep"))
    {
        ++idx;
        remain_ = true;
        return true;
    }

    return false;
}
