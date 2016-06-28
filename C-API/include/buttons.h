#pragma once


#include "resultcodes.h"


enum BUTTONS
{
    BUTTONS_ESC = 0,
    BUTTONS_DN,
    BUTTONS_MID,
    BUTTONS_OK,
    BUTTONS_UP
};


typedef void buttonsCallback(enum BUTTONS button, bool pressed);

enum RESULT setButtonsCallback(buttonsCallback *callback);

enum RESULT handleButtons();
