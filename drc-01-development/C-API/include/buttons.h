#pragma once


#include "resultcodes.h"
#include <stddef.h>


enum BUTTON
{
    BUTTON_ESC = 0,
    BUTTON_DN = 1,
    BUTTON_MID = 2,
    BUTTON_OK = 3,
    BUTTON_UP = 4
};


typedef void buttonsCallback(enum BUTTON button, bool pressed);

enum RESULT setButtonsCallback(buttonsCallback *callback);

enum RESULT handleButtons();

bool    getButtonState(enum BUTTON button, enum RESULT *res = NULL);

