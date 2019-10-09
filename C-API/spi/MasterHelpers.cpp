#include "spi/MasterHelpers.h"
#include "spi/GenericHelpers.h"


namespace drc01
{


void cmdCalcCrc(GenericCommand *cmd, uint8_t length)
{
    calcCrc((uint8_t *)cmd, length);
}


enum ERR rspErrorCode(GenericResponse *rsp)
{
    assert(rspIsError(rsp));
    return (enum ERR)rsp->length_;
}


int rspIsBusy(GenericResponse *rsp)
{
    return RSP_BUSY == rsp->length_;
}


int rspCheckCrc(GenericResponse *rsp)
{
    uint8_t length;
    if(rspIsBusy(rsp))
        length = sizeof(SpecialResponse);
    else if(rspIsError(rsp))
        length = sizeof(ErrorResponse);
    else
        length = rsp->length_;

    return checkCrc((uint8_t *)rsp, length);
}


}
