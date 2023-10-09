// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#include <c-api/helpers.h>
#include <stddef.h>


const char * getResultCodeString(RESULT result)
{
    static const char *resultStrings[] = {
        "RESULT_OK",
        "RESULT_API_NOT_OPEN",
        "RESULT_OBJECT_INVALID",
        "RESULT_ARGUMENT_INVALID",
        "RESULT_FUNCTION_INVALID",
        "RESULT_SERVICE_INVALID",
        "RESULT_COMM_TIMEOUT",
        "RESULT_SEE_ERRNO"
    };


    if(result >= RESULT_OK && result <= RESULT_SEE_ERRNO)
        return resultStrings[result];
    else
        return NULL;
}
