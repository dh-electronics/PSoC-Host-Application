#include "GenericHelpers.h"


static uint8_t crc(const uint8_t *data, uint8_t length)
{
    uint8_t crc = 0;
    uint8_t i;
    for(i = 0; i < length; ++i)
        crc += data[i];
    return crc;
}


void calcCrc(uint8_t *data, uint8_t length)
{
    --length;
    data[length] = crc(data, length);
}


int checkCrc(const uint8_t *data, uint8_t length)
{
    --length;
    return crc(data, length) == data[length];
}


int rspIsError(struct GenericResponse *rsp)
{
    return RSP_ERR <= rsp->length_;
}


uint8_t get_8(const uint8_t *data, uint8_t offset)
{
    return data[offset];
}


uint16_t get_16(const uint8_t *data, uint8_t offset)
{
    return ((uint16_t)(data[offset]) << 8) |
            (uint16_t)(data[offset + 1]);
}


uint32_t get_32(const uint8_t *data, uint8_t offset)
{
    return ((uint32_t)(data[offset]) << 24) |
            ((uint32_t)(data[offset + 1]) << 16) |
            ((uint32_t)(data[offset + 2]) << 8) |
            (uint32_t)(data[offset + 3]);
}


void set_8(uint8_t *data, uint8_t offset, uint8_t value)
{
    data[offset] = value;
}


void set_16(uint8_t *data, uint8_t offset, uint16_t value)
{
    data[offset] = (uint8_t)(value >> 8);
    data[offset + 1] = (uint8_t)(value);
}


void set_32(uint8_t *data, uint8_t offset, uint32_t value)
{
    data[offset] = (uint8_t)(value >> 24);
    data[offset + 1] = (uint8_t)(value >> 16);
    data[offset + 2] = (uint8_t)(value >> 8);
    data[offset + 3] = (uint8_t)(value);
}
