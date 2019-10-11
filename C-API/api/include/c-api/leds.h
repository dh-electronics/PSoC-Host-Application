#pragma once
#pragma GCC visibility push(default)

/** \file
 *  \brief Header for LED control
 */

#include "resultcodes.h"
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


/** LEDs in DRC02 front panel */
enum LED
{
    LED_RUN, ///< run LED (green)
    LED_LAN, ///< bus LED (green)
    LED_BUS, ///< lan LED (green)
    LED_ERR  ///< err LED (red)
};


/**
 * \brief control a LED
 *
 * Switch on/switch off a LED
 *
 * \param led choose LED to control
 * \param on pass requested state (on=!0 / off==0) of LED
 *
 * \return result code of requested operation
 */

enum RESULT writeLed(enum LED led, int on);


/**
 * \brief read state of LED
 *
 * Check if a LED is on/off.
 *
 * \param led choose LED to control
 * \param res pointer to pass RESULT of operation
 *
 * \return current state of LED (on=!0 / off==0)
 */
int  getLedState(enum LED led, enum RESULT *result = NULL);


#ifdef __cplusplus
}
#endif
#pragma GCC visibility pop

