#include "init.h"
#include "buttons.h"
#include "display.h"
#include "bounce.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>


using namespace std;


void testSwap()
{
    displayFill();
    displayFillRect(19, 6, 55, 47, true);
    displayFlush();
    for(uint16_t i = 0; i < 10; ++i)
    {
        displaySwap();
        usleep(250000);
    }
}


void testFillRect()
{
    displayFill();
    displayFlush();

    for(uint16_t i = 0; i < 100; ++i)
    {
        displayFillRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100,
                    rand() > RAND_MAX / 2);

        displayFlush();
        usleep(10000);
    }
}


void testInvertRect()
{
    displayFill();
    displayFlush();
    for(uint16_t i = 0; i < 100; ++i)
    {
        displayInvertRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100);
        displayFlush();
        usleep(10000);
    }
}


void testDrawRect()
{
    displayFill();
    displayFlush();
    for(uint16_t i = 0; i < 100; ++i)
    {
        displayDrawRect(
                    rand() % 200 - 100, rand() % 100 - 50,
                    rand() % 200, rand() % 100,
                    rand() > RAND_MAX / 2);

        displayFlush();
        usleep(10000);
    }
}


int main(int argc, char **argv)
{
    RESULT res = openApi();
    if(RESULT_OK != res)
        exit(1);

    res = displayEnable(true);
    if(RESULT_OK != res)
        exit(3);

    Bounce::run();
    testSwap();
    testFillRect();
    testDrawRect();
    testInvertRect();

    displayEnable(false);
    closeApi();
    exit(EXIT_SUCCESS);
}
