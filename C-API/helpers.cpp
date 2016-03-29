#include "helpers.h"
#include <stddef.h>


using namespace bigfish;


const char * bigfish::getResultCodeString(RESULT result)
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
