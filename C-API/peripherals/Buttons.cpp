#include "peripherals/Buttons.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <stdint.h>


using namespace drc01;


Buttons::Buttons(SpiProto &proto)
    : proto_(proto)
    , callback_(NULL)
{
    for(uint8_t i = 0; i < NUM_BUTTONS; ++i)
        states_[i] = false;
}


void Buttons::setCallback(buttonsCallback *callback)
{
    callback_ = callback;
}


RESULT Buttons::handleEvents()
{
    Command <0> cmd(CMD_BUTTONS_READ);
    Response <1> rsp;

    std::lock_guard<std::mutex> lock(accessMutex_);
    const RESULT res = proto_.xmit(cmd, rsp);
    if(RESULT_OK == res)
    {
        uint8_t evt = get_8(rsp.data(), 0);
        for(uint8_t i = 0; i < 2; ++i) // 2 events packed (an event in each tetrade)
        {
            const uint8_t idx = evt & 0x07;
            if(idx >= NUM_BUTTONS)
                break;

            const bool state = evt & 0x08;
            states_[idx] = state;

            if(callback_)
                callback_(BUTTON(idx), state);

            evt >>= 4;
        }
    }

    return res;
}


bool Buttons::getState(enum BUTTON button) const
{
    return states_[button];
}
