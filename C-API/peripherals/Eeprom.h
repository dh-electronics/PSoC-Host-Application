#pragma once


#include <peripherals/IEeprom.h>


namespace drc01
{


class SpiProto;


class Eeprom : public IEeprom
{
public:
    Eeprom(SpiProto &proto);

    virtual RESULT write(uint16_t *data, uint16_t byteOffset, uint8_t wordCount);

private:
    static const uint16_t EEPROM_WAITTIME = 64;

    SpiProto &              proto_;
};


} // namespace bigfish
