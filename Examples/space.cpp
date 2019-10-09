#include "space.h"
#include <c-api/display.h>
#include <c-api/buttons.h>
#include <c-api/leds.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


bool Space::running_;


static uint8_t bmpRocket[] = {
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

static uint8_t bmpAlien1[] = {
    0b00100000,0b10000000,
    0b00010001,0b00000000,
    0b00111111,0b10000000,
    0b01101110,0b11000000,
    0b11111111,0b11100000,
    0b10111111,0b10100000,
    0b10100000,0b10100000,
    0b00011011,0b00000000,
};


static uint8_t bmpAlien2[] = {
    0b00100000,0b10000000,
    0b10010001,0b00100000,
    0b10111111,0b10100000,
    0b11101110,0b11100000,
    0b11111111,0b11100000,
    0b00111111,0b10000000,
    0b00100000,0b10000000,
    0b01000000,0b01000000,
};




void Space::callback(enum BUTTON button, bool pressed)
{
    static bool escPressed = false;
    static bool okPressed = false;

    const char *buttonNames[] = { "DN_LEFT", "DN_RIGHT", "MID", "UP_LEFT", "UP_RIGHT" };
    if(button == BUTTON_DN_RIGHT)
        escPressed = pressed;

    if(button == BUTTON_DN_LEFT)
        okPressed = pressed;

    if(escPressed && okPressed)
        running_ = false;

    printf("%s : %s\n", buttonNames[button], pressed ? "pressed" : "released");
    fflush(stdout);
}


void Space::run()
{
    printf("Press ESC + OK to finish.\n");
    running_ = true;
    handleButtons();

    RESULT res = setButtonsCallback(callback);
    if(RESULT_OK != res)
        return;

    Bitmap rocket(8, 16, bmpRocket);
    Bitmap alien1(11, 8, bmpAlien1);
    Bitmap alien2(11, 8, bmpAlien2);

    uint8_t count = 0;
    bool led = false;
    for(uint16_t i = 0; running_; ++i)
    {
        displayFill();

        const int xr = (i >> 1) % 145 - 16;
        const int yr = 80 - i % 97;
        displayBitmap( xr, yr, rocket);

        const int xa = i % 152 - 12;
        const int ya = i % 80 - 8;
        Bitmap &bmp = (i >> 2) & 1 ? alien1 : alien2;
        displayBitmap(xa, ya, bmp);
        displayFlush();

        {
            const int deltax = xa - xr;
            const int deltay = ya - yr;
            const int dist2 = deltax*deltax + deltay*deltay;
            if(rand() % 900 > dist2)
            {
                led = true;
                writeLed(LED_ERR, true);
            }
        }

        usleep(10000);
        handleButtons();

        if(led)
            writeLed(LED_ERR, false);

        ++count;
        writeLed(LED_RUN, count & 2);
        writeLed(LED_BUS, count & 4);
        writeLed(LED_LAN, count & 8);
    }

    writeLed(LED_ERR, false);
    writeLed(LED_RUN, false);
    writeLed(LED_BUS, false);
    writeLed(LED_LAN, false);

}

