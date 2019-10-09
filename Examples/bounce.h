#pragma once


#include <c-api/buttons.h>
#include <stdint.h>


class Bounce
{
public:
    static const int CIRCLE = 64;
    static const int MAXSIZE = 12;
    static const int HALFSIZE = MAXSIZE/2;
    static const int X_MAX = (128 - HALFSIZE) << 8;
    static const int X_MIN = (HALFSIZE + 1) << 8;
    static const int Y_MAX = (64 - HALFSIZE) << 8;
    static const int Y_MIN = X_MIN;

    static void run();

private:
    static void buttonsCallback(enum BUTTON button, bool pressed);
    static int  sin(uint8_t angle) { return sin_[angle % CIRCLE]; }
    static int  cos(uint8_t angle) { return sin_[(angle + CIRCLE/4)%CIRCLE]; }
    static void drawSnake(bool color);

    static int      bX_[MAXSIZE], bY_[MAXSIZE];
    static int      i_;
    static uint8_t  angle_;
    static int      sin_[CIRCLE];
    static bool     running_;
    static bool     up_, down_;
};
