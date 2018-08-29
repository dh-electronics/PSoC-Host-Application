#include "Api.h"
#include <init.h>
#include <string.h>
#include <stdio.h>
#include <buttons.h>
#include <Poco/Timestamp.h>
#include <Poco/Thread.h>


using namespace Poco;


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
            Thread::sleep(50);
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