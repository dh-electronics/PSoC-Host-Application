#ifndef LEDS_H_
#define LEDS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "resultcodes.h"


enum LED
{
    LED_RS232_A, LED_RS232_B,
    LED_CAN,
    LED_READY, LED_STATUS, LED_ALARM, LED_UPLINK
};


enum RESULT;


enum RESULT writeLed(enum LED led, int on);


int         getLedState(enum LED led, enum RESULT *result);


#ifdef __cplusplus
}
#endif

#endif // LEDS_H_
