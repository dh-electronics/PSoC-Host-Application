#include "bounce.h"
#include "display.h"
#include "leds.h"
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <stdio.h>


using namespace std;


int  Bounce::i_;
int  Bounce::bX_[Bounce::MAXSIZE];
int  Bounce::bY_[Bounce::MAXSIZE];
bool Bounce::running_;
bool Bounce::up_;
bool Bounce::down_;
int  Bounce::sin_[Bounce::CIRCLE];
uint8_t Bounce::angle_;


void Bounce::buttonsCallback(enum BUTTON button, bool pressed)
{
    // cout << "Button " << uint16_t(button) << ' ' << pressed << endl;
    switch(button)
    {
    case BUTTON_ESC:
        running_ = false;
        break;

    case BUTTON_DN:
        down_ = pressed;
        break;

    case BUTTON_UP:
        up_ = pressed;
        break;

    default:
        break;
    }
}


void Bounce::drawSnake(bool color)
{
    for(int i = 0; i < MAXSIZE; ++i)
    {
        const int size = MAXSIZE - i;
        const int idx = (i_ + size) % MAXSIZE;
        const int hsize = size >> 1;

        int screenX = (bX_[idx] >> 8) - hsize;
        int screenY = (bY_[idx] >> 8) - hsize;

        displayFillRect(screenX, screenY, size, size, color);
    }
}


void Bounce::run()
{
    printf("Press ESC to finish.\n");

    for(uint16_t a = 0; a < CIRCLE; ++a)
        sin_[a] = 1024 * ::sin(a * 2 * M_PI / CIRCLE);

    running_ = true;
    angle_ = 64;
    up_ = down_ = false;
    for(int i = 0; i < MAXSIZE; ++i)
    {
        bX_[i] = 64 << 8;
        bY_[i] = 32 << 8;
    }
    i_ = MAXSIZE - 1;

    handleButtons();
    RESULT res = setButtonsCallback(buttonsCallback);
    if(RESULT_OK != res)
        return;

    bool led = false;
    displayFill();
    displayDrawRect(0, 0, 128, 64, true);
    displayFlush();

    uint8_t count = 0;
    while(running_)
    {
        drawSnake(true);
        displayFlush();
        // usleep(10000);

        handleButtons();
        drawSnake(false);

        if(led)
        {
            writeLed(LED_ERR, false);
            led = false;
        }

        if(up_)
            ++angle_;
        if(down_)
            --angle_;

        const int oldX = bX_[i_];
        const int oldY = bY_[i_];
        if(++i_ >= MAXSIZE)
            i_ = 0;

        int newX = oldX + Bounce::sin(angle_);
        int newY = oldY + Bounce::cos(angle_);

        if(newX < X_MAX && newX >= X_MIN)
        {
            bX_[i_] = newX;
        }
        else
        {
            bX_[i_] = oldX;
            angle_ = -angle_;
            led = true;
        }

        if(newY < Y_MAX && newY >= Y_MIN)
        {
            bY_[i_] = newY;
        }
        else
        {
            bY_[i_] = oldY;
            angle_ = CIRCLE/2 - angle_;
            led = true;
        }

        if(led)
            writeLed(LED_ERR, true);

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
