#include "Display.h"
#include "Buttons.h"
#include "Leds.h"
#include "Api.h"
#include <stdint.h>
#include <init.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


static const char *strHelp = "ui-tool usage:"
                             "\n\n\tdisplay {some of the following} ?loop"
                             "\n\t\tenable\t- enable display"
                             "\n\t\tdisable\t- disable display"
                             "\n\t\tfill ?(1|0)\t- fill display"
                             "\n\t\tfillrect ?X ?Y ?W ?H ?(1|0)\t- fill rect"
                             "\n\t\tshow\t- show display"
                             "\n\n\tbuttons\t- poll buttons"
                             "\n\n\tleds {(RUN|LAN|BUS|ERR) ?(1|0)} - control leds"
                             "\n\n\tidle\t- idle API"
                             "\n\n\tloop\t- remain in the app, continue polling buttons"
                             "\n\n\tkeep\t- when closing the app do not close the API"
                             "\n\nLegend: ?OPTIONAL_PARAM, (either this|or this), {number of times}";


bool running = true;


void sigHandler(int)
{
    running = false;
}


int main(int argc, char **argv)
{
    // setting up signals
    {
        struct sigaction sa;
        sigemptyset(&sa.sa_mask);
        sa.sa_handler = sigHandler;
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGHUP, &sa, NULL);
    }

    if(argc == 1)
    {
        printf(strHelp);
        exit(EXIT_SUCCESS);
    }

    RESULT res = openApi();
    if(RESULT_OK != res)
        exit(EXIT_FAILURE);

    for(uint16_t idx = 1; idx < argc;)
    {
        if(!(Display::parseArgs(argc, argv, idx) ||
             Buttons::parseArgs(argc, argv, idx) ||
             Leds::parseArgs(argc, argv, idx) ||
             Api::parseArgs(argc, argv, idx)))
        {
            printf("Unknown argument %hd: \"%s\". Quitting.", idx, argv[idx]);
            break;
        }
    }

    if(!Api::mustRemain())
        closeApi();
}
