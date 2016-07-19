#include "init.h"
#include "buttons.h"
#include "display.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>


bool stop = false;


static void butCallback(enum BUTTON button, bool pressed)
{
    switch(button)
    {
    case BUTTON_ESC:
        stop = true;
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



void testFillRect()
{
    displayFill();
    for(uint16_t i = 0; i < 1000; ++i)
    {
        displayFillRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100,
                    rand() > RAND_MAX / 2);
        displayFlush();
        usleep(100000);
    }
}


void testInvertRect()
{
    displayFill();
    for(uint16_t i = 0; i < 1000; ++i)
    {
        displayInvertRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100);
        displayFlush();
        usleep(100000);
    }
}


void testDrawRect()
{
    displayFill();
    for(uint16_t i = 0; i < 1000; ++i)
    {
        displayDrawRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100,
                    rand() > RAND_MAX / 2);

        displayFlush();
        usleep(100000);
    }
}


int main(int argc, char **argv)
{
    RESULT res = openApi();
    if(RESULT_OK != res)
        exit(1);

    res = setButtonsCallback(butCallback);
    if(RESULT_OK != res)
        exit(2);

    res = displayEnable(true);
    if(RESULT_OK != res)
        exit(3);

    testFillRect();
    testDrawRect();
    testInvertRect();

    displayEnable(false);
    closeApi();
    exit(EXIT_SUCCESS);
}
