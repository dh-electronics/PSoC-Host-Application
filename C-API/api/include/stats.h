#pragma once

/** \file
 *  \brief Handle SPI communication statistics
 *
 *  In the SPI communication between the DHCOM and the MCU can be errors. Check
 *  on the stats if there are serious problems (hardware or software).
 */

#include "resultcodes.h"
#include <stddef.h>

/** project name of 'C-API' library */
#define API_NAME        "C-API"

/** software version of C-API library */
#define API_VERSION     8

/** software revision of C-API library */
#define API_REVISION    169

/**
 * \brief Get count of checksum errors of SPI command detected by MCU
 *
 * If the MCU detects a checksum error in the incoming command this counter is
 * incremented. Not critical, but must not rise too fast.
 *
 * \param result pointer to pass RESULT of operation
 *
 * \return count of crc errors
 */

unsigned int getCrcErrorCounter(enum RESULT *result = NULL);

/**
 * \brief Get count of the communication errors on SPI bus
 *
 * Error count is not critical, but if the number rises too fast there may be a
 * hardware problem with the SPI interface.
 *
 * \param result pointer to pass RESULT of operation
 *
 * \return count of communication errors
 */

unsigned int getResponseErrorCounter(enum RESULT *result = NULL);

/**
 * \brief Get count of detected busy states of MCU on SPI command
 *
 * The counter is incremented if the MCU is in the middle of a command
 * processing and already the next order comes. May not rise quickly, otherwise
 * there is a software problem (should be optimized).
*/
unsigned int getBusyCounter(enum RESULT *result = NULL);

/**
 * \brief Get count of SPI communication timeouts (after many attempts)
 *
 * This counter is incremented when, after many attempts, the communication with
 * the MCU is missing. Must be zero. If rising - there is a SW or HW problem.
 *
 * \param result pointer to pass RESULT of operation
 *
 * \return count of SPI communication timeouts
 */
unsigned int getTimeoutsCounter(enum RESULT *result = NULL);

/**
 * \brief Get count of MCU resets
 *
 * If the MCU no longer responds to periodic ping, DHCOM resets it. Must
 * be zero and if it rises there is a problem with HW or MCU SW.
 *
 * \param result pointer to pass RESULT of operation
 *
 * \return count MCU resets through DHCOM
 */
unsigned int getResetsCounter(enum RESULT *result = NULL);

/**
 * \brief Reset error stats
 *
 * Reset error counters (stats) of the SPI communication.
 *
 * \param result pointer to pass RESULT of operation
 */
void resetStats(enum RESULT *result = NULL);

/**
 * \brief get software version of C-API library
 *
 * \param result pointer to pass RESULT of operation
 *
 * \return integer with software version code.
 */
unsigned int getApiVersion(enum RESULT *result = NULL);

/**
 * \brief get mcu firmware version
 *
 * \param result pointer to pass RESULT of operation
 *
 * \return integer with software version code.
 */
unsigned int getPicVersion(enum RESULT *result = NULL);

/**
 * \brief get hardware version of base board
 *
 * \param result pointer to pass RESULT of operation
 *
 * \return integer with hardware version of the base board.
 */
unsigned int getHwRevision(enum RESULT *result = NULL);

