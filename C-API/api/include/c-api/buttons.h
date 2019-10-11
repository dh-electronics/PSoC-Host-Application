#pragma once
#pragma GCC visibility push(default)

/**
 * \file buttons.h
 * \brief Header for accessing the PCAP buttons
 *
 */

#include <stddef.h>
#include <stdbool.h>
#include "resultcodes.h"


#ifdef __cplusplus
extern "C" {
#endif


/** cap. touch buttons of DRC02/Bigfish front panel */
enum BUTTON
{
    BUTTON_DN_LEFT  = 0, ///< DN_LEFT (left) button
    BUTTON_DN_RIGHT = 1, ///< DN_RIGHT (right) button
    BUTTON_MID      = 2, ///< MID (OK) button
    BUTTON_UP_LEFT  = 3, ///< UP_LEFT (up) button
    BUTTON_UP_RIGHT = 4  ///< UP_RIGHT (down) button
};


typedef void buttonsCallback(enum BUTTON button, bool pressed);

/**
 * \brief set callback for PCAP buttons
 *
 * Sets a new callback for PCAP buttons. Can also be called with NULL.
 * If not NULL the passed function is called for each PCAP button
 * event (pressed and released).
 *
 * \param callback function pointer to callback function
 *
 * \return result code of requested operation
 */
enum RESULT setButtonsCallback(buttonsCallback *callback);

/**
 * \brief trigger PCAP buttons measurement
 *
 * Reads the PCAP buttons states, therefore it must be called regularly.
 * When PCAP buttons states are changing, the callback for each state change of
 * each PCAP button is called.
 *
 * \return result code of requested operation
 */
enum RESULT handleButtons();

/**
 * \brief read state of a specific PCAP button
 *
 * For most applications it is recommended to work with handleButtons() and a
 * callback instead of using getButtonState().
 *
 * \param button pass the PCAP button of interest
 * \param res pointer to pass RESULT of operation
 *
 * \return state of button (true==pressed/false==not pressed)
 */
bool getButtonState(enum BUTTON button, enum RESULT *res = NULL);


#ifdef __cplusplus
}
#endif
#pragma GCC visibility pop

