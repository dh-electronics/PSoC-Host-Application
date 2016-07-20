#pragma once


#include "buttons.h"


class Bounce
{
public:
    static const int MAXSIZE = 8;
    static const int HALFSIZE = MAXSIZE/2;
    static const int X_MAX = (128 - HALFSIZE) << 8;
    static const int X_MIN = (HALFSIZE + 1) << 8;
    static const int Y_MAX = (64 - HALFSIZE) << 8;
    static const int Y_MIN = X_MIN;

    static void run();

private:
    static void buttonsCallback(enum BUTTON button, bool pressed);

    static int  player_;
    static int  bX_, bY_, bVX_, bVY_;
    static bool running_;
};
