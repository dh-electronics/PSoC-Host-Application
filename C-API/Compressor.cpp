#include "Compressor.h"
#include "peripherals/Display.h"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace drc01;
using namespace std;


/*
static void dump(const uint8_t *src, const uint8_t *src_end)
{
    for(const uint8_t *s = src; s != src_end; ++s)
    {
        if(0 == (s - src)%128)
            putchar('\n');
        printf("%.2hhx ", *s);
    }
}
*/


Compressor::Compressor(uint8_t *dst, const uint8_t *src)
    : dst_start_(dst)
    , dst_(dst)
    , src_(src)
    , src_end_(src + Display::BUFSIZE)
{
}


uint16_t Compressor::compress()
{
    // cout << endl;
    // dump(src_, src_end_);

    {   // compression always starts from coding zeros
        const uint16_t zeros = countZeros(src_);        // count the zero bytes
        src_ += zeros;
        // cout << "zeros:" << zeros << endl;
        codeNumber(zeros);                              // code the number of zeros
    }

    uint16_t nonZeros = 0;
    const uint8_t *srcAhead = src_;
    while(srcAhead != src_end_)
    {
        nonZeros += countNonZeros(srcAhead);            // count the non-zero bytes
        srcAhead = src_ + nonZeros;

        const uint16_t zeros = countZeros(srcAhead);    // count zero bytes onwards

        if(zeros > 2)
        {   // it is more optimal to code more than two zeros separately
            codeNumber(nonZeros);
            copyData(nonZeros);
            // cout << "non-zeros:" << nonZeros << endl;
            // cout << "zeros:" << zeros << endl;
            nonZeros = 0;

            codeNumber(zeros);
            srcAhead += zeros;
            src_ = srcAhead;
        }
        else if(zeros)
        {   // less than three zeros is not optimal to code separately
            srcAhead += zeros;
            nonZeros += zeros;
        }
    }

    if(nonZeros)
    {
        // cout << "non-zeros:" << nonZeros << endl;
        codeNumber(nonZeros);
        copyData(nonZeros);
    }

    // dump(dst_start_, dst_);
    // cout << "length" << dst_ - dst_start_;
    return dst_ - dst_start_;
}


uint16_t Compressor::countZeros(const uint8_t *at) const
{
    uint16_t num = 0;
    for(const uint8_t *p = at; !*p && p != src_end_; ++p)
        ++num;
    return num;
}


uint16_t Compressor::countNonZeros(const uint8_t *at) const
{
    uint16_t num = 0;
    for(const uint8_t *p = at; *p && p != src_end_; ++p)
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
