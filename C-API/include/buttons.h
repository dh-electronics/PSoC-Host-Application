#pragma once


#include "resultcodes.h"
#include <stddef.h>


enum BUTTON
{
    BUTTON_DN_LEFT  = 0,      // was BUTTON_UP
    BUTTON_DN_RIGHT = 1,      // was BUTTON_DN
    BUTTON_MID      = 2,
    BUTTON_UP_LEFT  = 3,      // was BUTTON_ESC
    BUTTON_UP_RIGHT = 4       // was BUTTON_OK
};


typedef void buttonsCallback(enum BUTTON button, bool pressed);

enum RESULT setButtonsCallback(buttonsCallback *callback);

enum RESULT handleButtons();

bool    getButtonState(enum BUTTON button, enum RESULT *res = NULL);

