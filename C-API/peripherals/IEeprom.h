#pragma once


#include <stdint.h>
#include <c-api/resultcodes.h>


namespace drc01
{


class IEeprom
{
public:
    virtual ~IEeprom() {}

    virtual RESULT write(uint16_t *data, uint16_t byteOffset, uint8_t wordCount) =0;
};


}
