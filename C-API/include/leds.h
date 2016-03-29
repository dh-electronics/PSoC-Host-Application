#ifndef LEDS_H_
#define LEDS_H_

#ifdef __cplusplus
extern "C" {
#endif


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


#ifdef __cplusplus
}
#endif

#endif // LEDS_H_
