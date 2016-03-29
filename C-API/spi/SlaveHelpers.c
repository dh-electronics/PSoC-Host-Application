#include "SlaveHelpers.h"


int cmdCheckCrc(struct GenericCommand *cmd, uint8_t length)
{
    return checkCrc((uint8_t *)cmd, length);
}


void rspCalcCrc(struct GenericResponse *rsp)
{
    switch(rsp->length_)
    {
    case RSP_BUSY:
        calcCrc((uint8_t *)rsp, sizeof(struct SpecialResponse));
        break;

    default:
        if(rspIsError(rsp))
            calcCrc((uint8_t *)rsp, sizeof(struct ErrorResponse));
        else
            calcCrc((uint8_t *)rsp, rsp->length_);
        break;
    }
}
