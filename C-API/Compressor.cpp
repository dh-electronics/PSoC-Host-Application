#include "Compressor.h"
#include "peripherals/Display.h"
#include <cstring>

using namespace drc01;


Compressor::Compressor(uint8_t *dst, const uint8_t *src)
    : dst_start_(dst)
    , dst_(dst)
    , src_(src)
    , src_end_(src + Display::BUFSIZE)
{
}


uint16_t Compressor::compress()
{
    {   // compression always starts from coding zeros
        const uint16_t zeros = countZeros(src_);        // count the zero bytes
        src_ += zeros;
        codeNumber(zeros);                              // code the number of zeros
    }

    uint16_t nonZeros = 0;
    const uint8_t *srcAhead = src_;
    while(!atEnd())
    {
        nonZeros += countNonZeros(srcAhead);            // count the non-zero bytes
        srcAhead = src_ + nonZeros;

        const uint16_t zeros = countZeros(srcAhead);    // count zero bytes onwards
        if(zeros > 2)
        {   // it is more optimal to code more than two zeros separately
            codeNumber(nonZeros);
            copyData(nonZeros);
            nonZeros = 0;

            codeNumber(zeros);
            src_ += zeros;
            srcAhead = src_;
        }
        else if(zeros)
        {   // less than three zeros is not optimal to code separately
            srcAhead += zeros;
            nonZeros += zeros;
        }
    }

    if(nonZeros)
    {
        codeNumber(nonZeros);
        copyData(nonZeros);
    }

    return dst_ - dst_start_;
}


uint16_t Compressor::countZeros(const uint8_t *at) const
{
    uint16_t num = 0;
    for(const uint8_t *p = at; *p == 0; ++p);
        ++num;
    return num;
}


uint16_t Compressor::countNonZeros(const uint8_t *at) const
{
    uint16_t num = 0;
    for(const uint8_t *p = at; *p != 0; ++p);
        ++num;
    return num;
}


void Compressor::codeNumber(uint16_t num)
{
    const uint8_t highByte = num >> 8;
    const uint8_t lowByte = uint8_t(num);
    if(highByte || (lowByte & 0x80))
        codeByte(highByte | 0x80);

    codeByte(lowByte);
}


void Compressor::copyData(uint16_t num)
{
    memcpy(dst_, src_, num);
    dst_ += num;
    src_ += num;
}
