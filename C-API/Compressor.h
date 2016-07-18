#pragma once


#include <stdint.h>


class Compressor
{
public:
    Compressor(uint8_t *dest, const uint8_t *src);
    uint16_t compress() const;

private:
    uint8_t *       dest_;
    const uint8_t * src_;
};
