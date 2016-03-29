#include "Display.h"
#include "peripherals/Gpios.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <string.h>
#include <cassert>


using namespace bigfish;
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


RESULT Display::text(const char *text)
{
    assert(text);
    const size_t textLength = strlen(text);
    const uint8_t chunkSize = SPI_PACKET_SIZE - 2;
    for(size_t pos = 0; pos < textLength; )
    {
        const size_t symbolsLeft = textLength - pos;
        const uint8_t symbolsToXfer = (symbolsLeft > chunkSize) ? chunkSize : symbolsLeft;

        CommandUnion cu;
        cu.cmd.command_ = CMD_DISPLAY_TEXT;
        uint8_t cmdLength = symbolsToXfer + 2;
        memcpy(cu.cmd.data_, &text[pos], symbolsToXfer);
        cmdCalcCrc(&cu.cmd, cmdLength);

        ResponseUnion ru;
        const RESULT res = proto_.xmit(&cu.cmd, cmdLength, &ru.rsp, 2, SpiProto::CMD_WAIT_MS + DISPLAY_SYMBOL_WAIT_MS * symbolsToXfer);
        if(RESULT_OK != res)
            return res;

        pos += symbolsToXfer;
    }

    return RESULT_OK;
}


RESULT Display::show()
{
    Command <0> cmd(CMD_DISPLAY_SHOW);
    Response <0> rsp;
    return proto_.xmit(cmd, rsp, DISPLAY_SHOW_WAIT_MS);
}
