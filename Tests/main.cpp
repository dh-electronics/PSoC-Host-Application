#include "init.h"
#include "buttons.h"
#include "display.h"
#include "bounce.h"
#include "bitmap.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>


using namespace std;


uint8_t bmpRocket[] = {
    0b00011000,
    0b00100100,
    0b01000010,
    0b01011010,
    0b01100110,
    0b01100110,
    0b01011010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b10100101,
    0b10100101,
    0b10100101,
    0b10100101,
    0b11111111,
    0b10100101,
};

uint8_t bmpAlien1[] = {
    0b00100000,0b10000000,
    0b00010001,0b00000000,
    0b00111111,0b10000000,
    0b01101110,0b11000000,
    0b11111111,0b11100000,
    0b10111111,0b10100000,
    0b10100000,0b10100000,
    0b00011011,0b00000000,
};


uint8_t bmpAlien2[] = {
    0b00100000,0b10000000,
    0b10010001,0b00100000,
    0b10111111,0b10100000,
    0b11101110,0b11100000,
    0b11111111,0b11100000,
    0b00111111,0b10000000,
    0b00100000,0b10000000,
    0b01000000,0b01000000,
};


void testBitmap()
{
    Bitmap rocket(8,16);
    memcpy(rocket.data, bmpRocket, rocket.size);

    Bitmap alien1(11,8);
    memcpy(alien1.data, bmpAlien1, alien1.size);

    Bitmap alien2(11,8);
    memcpy(alien2.data, bmpAlien2, alien2.size);

    displayFill();

    for(uint16_t i = 0; true; ++i)
    {
        const int xr = (i >> 1) % 145 - 16;
        const int yr = 80 - i % 97;
        displayBitmap( xr, yr, rocket);

        const int xa = i % 152 - 12;
        const int ya = i % 80 - 8;
        Bitmap &bmp = (i >> 2) & 1 ? alien1 : alien2;
        displayBitmap( xa, ya, bmp);
        displayFlush();

        usleep(10000);
        displayBitmap( xr, yr, rocket);
        displayBitmap( xa, ya, bmp);
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
    testBitmap();
    testSwap();
    testFillRect();
    testDrawRect();
    testInvertRect();

    displayEnable(false);
    closeApi();
    exit(EXIT_SUCCESS);
}
