#include "init.h"
#include "buttons.h"
#include "display.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;


static void butCallback(enum BUTTON button, bool pressed)
{
    switch(button)
    {
    case BUTTON_ESC:
        break;

    case BUTTON_DN:
        break;

    case BUTTON_MID:
        break;

    case BUTTON_OK:
        break;

    case BUTTON_UP:
        break;
    }
}


void testSwap()
{
    displayFill();
    displayFillRect(19, 6, 55, 47, true);
    displayFlush();
    for(uint16_t i = 0; i < 10; ++i)
    {
        displaySwap();
        usleep(500000);
    }
}


void testFillRect()
{

    for(uint16_t i = 0; i < 10; ++i)
    {
        const bool white = rand() > RAND_MAX / 2;
        displayFill(white);
        displayFlush();
        usleep(500000);

        displayFillRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100,
                    !white);

        displayFlush();
        usleep(200000);
    }
}


void testInvertRect()
{
    for(uint16_t i = 0; i < 10; ++i)
    {
        displayFill(rand() > RAND_MAX / 2);
        displayFlush();
        usleep(500000);

        displayInvertRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100);
        displayFlush();
        usleep(200000);
    }
}


void testDrawRect()
{
    for(uint16_t i = 0; i < 10; ++i)
    {
        const bool white = rand() > RAND_MAX / 2;
        displayFill(white);
        displayFlush();
        usleep(500000);

        displayDrawRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100,
                    !white);

        displayFlush();
        usleep(200000);
    }
}


int main(int argc, char **argv)
{
    RESULT res = openApi();
    if(RESULT_OK != res)
        exit(1);

    /* res = setButtonsCallback(butCallback);
    if(RESULT_OK != res)
        exit(2);
    */

    res = displayEnable(true);
    if(RESULT_OK != res)
        exit(3);

    testSwap();
    testFillRect();
    testDrawRect();
    testInvertRect();

    displayEnable(false);
    closeApi();
    exit(EXIT_SUCCESS);
}
