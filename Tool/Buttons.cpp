#include "Buttons.h"
#include <string.h>
#include <stdio.h>
#include <Poco/Timestamp.h>
#include <Poco/Thread.h>


using namespace Poco;


void Buttons::callback(BUTTON button, bool pressed)
{
    static const char *strButt[] = {"UP", "LEFT", "MID", "DOWN", "RIGHT"};
    printf("%s:%s\n", strButt[button], pressed ? "pressed" : "released");
    fflush(stdout);
}


bool Buttons::parseArgs(int, char **argv, uint16_t &idx)
{
    if(0 != strcmp(argv[idx], "buttons"))
        return false;

    ++idx;
    setButtonsCallback(callback);
    handleButtons();

    return true;
}
