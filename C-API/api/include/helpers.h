#pragma once

/**
 * \file helpers.h
 * \brief Helpers
 *
 */

#include "resultcodes.h"


/**
 *  \brief Translate enum RESULT in a printable char string
 *
 *  \param result - pass the returned result of a C-API call
 *  \return pointer to char string
 */

const char * getResultCodeString(RESULT result);

