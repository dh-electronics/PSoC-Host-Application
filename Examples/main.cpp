#include <c-api/init.h>
#include <c-api/buttons.h>
#include <c-api/display.h>
#include <c-api/leds.h>
#include "bounce.h"
#include "space.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
        usleep(100000);
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


void testFullScreenBitmap()
{
    uint8_t bmpData[1024];
    for(int i = 0; i < 64; ++i)
    {
       const uint8_t pattern = i & 1 ? 0x55 : 0xAA;
       memset(bmpData + i*16, pattern, 16);
    }
    Bitmap bmp(128, 64, bmpData);

    displayFill();
    for(int i = 0; i < 10; ++i)
    {
        displayBitmap(0,0,bmp);
        displayFlush();
    }
}


int main(int, char **)
{
    RESULT res = openApi();
    if(RESULT_OK != res)
        exit(1);

    writeLed(LED_RUN, true);

    res = displayEnable(true);
    if(RESULT_OK != res)
        exit(3);

    testFullScreenBitmap();

    testSwap();
    testFillRect();
    testDrawRect();
    testInvertRect();
    Bounce::run();
    Space::run();

    displayEnable(false);
    closeApi();
    exit(EXIT_SUCCESS);
}
