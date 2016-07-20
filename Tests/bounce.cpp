#include "bounce.h"
#include "display.h"
#include <unistd.h>


int  Bounce::player_;
int  Bounce::bX_, Bounce::bY_;
int  Bounce::bVX_, Bounce::bVY_;
bool Bounce::running_;


void Bounce::buttonsCallback(enum BUTTON button, bool pressed)
{
    switch(button)
    {
    case BUTTON_ESC:
        if(pressed)
            running_ = false;
        break;

    case BUTTON_DN:
        break;

    case BUTTON_UP:
        break;

    default:
        break;
    }
}


void Bounce::run()
{
    running_ = true;
    bX_ = 10 << 8;
    bY_ = 32 << 8;
    bVX_ = 300;
    bVY_ = -100;

    RESULT res = setButtonsCallback(buttonsCallback);
    if(RESULT_OK != res)
        return;

    displayFill();
    displayDrawRect(0, 0, 128, 64, true);
    displayFlush();

    int size = 0;
    while(running_)
    {
        int screenX = (bX_ >> 8) - size;
        int screenY = (bY_ >> 8) - size;
        const int dsize = size << 1;

        displayDrawRect(screenX, screenY, dsize, dsize, true);
        displayFlush();
        // usleep(10000);

        handleButtons();
        displayDrawRect(screenX, screenY, dsize, dsize, false);

        int newX = bX_ + bVX_;
        if(newX < X_MAX && newX >= X_MIN)
            bX_ = newX;
        else
            bVX_ = -bVX_;

        int newY = bY_ + bVY_;
        if(newY < Y_MAX && newY >= Y_MIN)
            bY_ = newY;
        else
            bVY_ = -bVY_;

        if(++size > HALFSIZE)
            size = 0;
    }
}
