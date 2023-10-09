/* SPDX-License-Identifier: BSD-3-Clause */
/* SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH */

#pragma once
#pragma GCC visibility push(default)

/**
 * \file helpers.h
 * \brief Helpers
 *
 */

#include "resultcodes.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 *  \brief Translate enum RESULT in a printable char string
 *
 *  \param result - pass the returned result of a C-API call
 *  \return pointer to char string
 */

const char * getResultCodeString(RESULT result);


#ifdef __cplusplus
}
#endif
#pragma GCC visibility pop

