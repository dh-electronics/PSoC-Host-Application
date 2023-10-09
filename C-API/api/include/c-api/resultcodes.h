/* SPDX-License-Identifier: BSD-3-Clause */
/* SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH */

#pragma once
#pragma GCC visibility push(default)

/** \file
 *  \brief Header for result code handling
 */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief Enumeration for common return codes from <b>C-API</b> methods.
 *
 * Most of the class methods in the library return the RESULT code to reflect
 * the success of the requested operation.
 */
enum RESULT
{
    RESULT_OK,                  ///< successfully executed
    RESULT_API_NOT_OPEN,        ///< C-API not open, use openApi()
    RESULT_OBJECT_INVALID,      ///< non-existing object
    RESULT_ARGUMENT_INVALID,    ///< MCU Firmware reports a unexpected/invalid command argument
    RESULT_FUNCTION_INVALID,    ///< Got a unknown result code from MCU Firmware
    RESULT_SERVICE_INVALID,     ///< service not supported by MCU Firmware
    RESULT_COMM_TIMEOUT,        ///< Host to PSoC MCU timeout of spi communication
    RESULT_SEE_ERRNO            ///< Inspect errno for further failure information
};


#ifdef __cplusplus
}
#endif
#pragma GCC visibility pop

