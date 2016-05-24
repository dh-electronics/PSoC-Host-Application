#include "Display.h"
#include "peripherals/Gpios.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <string.h>
#include <cassert>


using namespace drc01;
using namespace std;


Display::Display(SpiProto &proto)
    : proto_(proto)
{
}


RESULT Display::enable(bool on)
{
    return proto_.gpioControl(RA4, on);
}


RESULT Display::clear()
{
    Command <2> cmd(CMD_DISPLAY_CLEAR);
    set_8(cmd.data(), 0, 0);
    set_8(cmd.data(), 1, 0);
    Response <0> rsp;
    return proto_.xmit(cmd, rsp, DISPLAY_CLEAR_WAIT_MS);
}


RESULT Display::invert(bool on)
{
    Command <1> cmd(CMD_DISPLAY_INVERSION);
    set_8(cmd.data(), 0, on ? 1 : 0);
    Response <0> rsp;
    return proto_.xmit(cmd, rsp);
}


RESULT Display::selectFont(int)
{   
    // may be implemented in future
    return RESULT_FUNCTION_INVALID;
}


RESULT Display::setPosition(int x, int y)
{
    Command <2> cmd(CMD_DISPLAY_SETPOS);
    set_8(cmd.data(), 0, uint8_t(x));
    set_8(cmd.data(), 1, uint8_t(y));
    Response <0> rsp;
    return proto_.xmit(cmd, rsp);
}


RESULT Display::bitmap(const unsigned char *data, int width, int height, BMP_FUNC func)
{
    assert(data);


    return RESULT_OK;
}

