#pragma once


#include <stdint.h>
#include <resultcodes.h>


namespace bigfish
{


class IEeprom
{
public:
    virtual RESULT write(uint16_t *data, uint16_t byteOffset, uint8_t wordCount) =0;
};


} // namespace bigfish
