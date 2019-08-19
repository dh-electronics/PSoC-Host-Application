#pragma once

/** \file
 *  \brief API initialization and termination
 *
 * Before the Linux operating system boots and the Cypress MCU is accessed by
 * the user application, Green LED will flash with 800mS interval. After the first
 * access, C-API takes over the flashing and flashes with 300mS interval.
 * This ends when user application opens API - Green LED will stop flashing and
 * control over the Led is transferred to the application.
 *
 * Note: Green LED will stop flashing when first API call is executed.
 * SW must then explicitly control the Green LED.
 *
 * \todo Clarify requested behavior of Green LED before and after API access.
 */


#include <resultcodes.h>

/**
 * \brief open API
 *
 * opens API, brings the Cypress MCU to active mode
 */

enum RESULT openApi();

/**
 * \brief close API
 *
 * brings Cypress MCU back to wait mode, cleans up resources
 */

enum RESULT closeApi();

/**
 * \brief idle API
 *
 * brings Green LED will start flashing with 300mS interval again.
 */

enum RESULT idleApi();
