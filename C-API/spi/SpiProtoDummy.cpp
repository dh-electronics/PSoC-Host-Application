#include "spi/SpiProto.h"


using namespace bigfish;


SpiProto::SpiProto()
{
}


SpiProto::~SpiProto()
{
}


void SpiProto::resetStats()
{
    // dummy
}


uint32_t SpiProto::getCrcErrorCounter() const
{
    return 0;
}


uint32_t SpiProto::getResponseErrorCounter() const
{
    return 0;
}


uint32_t SpiProto::getBusyCounter() const
{
    return 0;
}


uint32_t SpiProto::getTimeoutsCounter() const
{
    return 0;
}


bool SpiProto::start()
{
    return true;
}


void SpiProto::stop()
{
}


bool SpiProto::isRunning() const
{
    return true;
}


RESULT SpiProto::xmit(GenericCommand *, uint8_t, GenericResponse *, uint8_t)
{
    return RESULT_OK;
}


RESULT SpiProto::gpioControl(GPIO, bool)
{
    return RESULT_OK;
}


RESULT SpiProto::srControl(uint16_t, bool)
{
    return RESULT_OK;
}


void SpiProto::run()
{
}


void SpiProto::incCrcErrors()
{
}


void SpiProto::incResponseErrors()
{
}


void SpiProto::incBusy()
{
}


void SpiProto::incTimeouts()
{
}
