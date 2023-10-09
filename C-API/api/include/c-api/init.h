/* SPDX-License-Identifier: BSD-3-Clause */
/* SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH */

#pragma once
#pragma GCC visibility push(default)

/** \file
 *  \brief API initialization and termination
 *
 * Before the Linux operating system boots and the Cypress MCU is accessed by
 * the user application, Green RUN LED will flash with 800ms interval. This ends
 * when user application opens API: Green LED will stop flashing and the
 * control over the Led is transferred to the application.
 *
 * After the idleAPI call, C-API takes over the flashing and flashes with
 * 300ms interval.
 *
 * Note: Green RUN LED will stop flashing when first API call is executed.
 * The user application software must then explicitly control the Green RUN LED.
 *
 */


#include "resultcodes.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief open API
 *
 * opens API, brings the Cypress MCU to active mode
 *
 * \return result code of requested operation
 */
enum RESULT openApi();

/**
 * \brief close API
 *
 * Brings Cypress MCU back to wait mode, cleans up resources. The RUN LED will
 * start flashing with 800ms interval, again. The pcap touch button scan
 * is disabled.
 *
 * \return result code of requested operation
 */
enum RESULT closeApi();

/**
 * \brief idle API
 *
 * RUN LED will start flashing with 300mS interval. The pcap touch button
 * scan is disabled during idle mode. (Just don't use idleApi() it if pcap touch
 * buttons should always be active.)
 *
 * \return result code of requested operation
 */
enum RESULT idleApi();


#ifdef __cplusplus
}
#endif
#pragma GCC visibility pop

