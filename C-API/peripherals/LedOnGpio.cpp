#include "peripherals/LedOnGpio.h"
#include "spi/SpiProto.h"
#include <Poco/ScopedLock.h>


using namespace bigfish;
using namespace Poco;


LedOnGpio::LedOnGpio(SpiProto &proto, GPIO gpio)
    : proto_(proto)
    , gpio_(gpio)
{
}


RESULT LedOnGpio::write(bool on)
{
    ScopedLock <FastMutex> lock(accessMutex_);
    const RESULT res = proto_.gpioControl(gpio_, on);
    if(RESULT_OK == res)
        state_ = on;
    return res;
}
