#pragma once


#include "resultcodes.h"


enum LED
{
    LED_RUN,
    LED_LAN,
    LED_BUS,
    LED_ERR
};


enum RESULT;


enum RESULT writeLed(enum LED led, int on);


int         getLedState(enum LED led, enum RESULT *result);
