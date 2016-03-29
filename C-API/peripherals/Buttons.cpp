#include "peripherals/Buttons.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <Poco/ScopedLock.h>
#include <stdint.h>


using namespace bigfish;
using namespace Poco;


const BUTTONS Buttons::ZERO_BUTTONS = {0, 0, 0, 0, 0};


Buttons::Buttons(SpiProto &proto)
    : proto_(proto)
    , buttons_(ZERO_BUTTONS)
{
}


BUTTONS Buttons::read(RESULT *result)
{
    Command <0> cmd(CMD_BUTTONS_READ);
    Response <1> rsp;

    ScopedLock <FastMutex> lock(accessMutex_);
    const RESULT res = proto_.xmit(cmd, rsp);
    if(RESULT_OK == res)
    {
        union {
          BUTTONS bts;
          uint8_t raw;
        };
        raw = get_8(rsp.data(), 0);
        buttons_ = bts;
    }

    if(result)
        *result = res;

    return buttons_;
}
