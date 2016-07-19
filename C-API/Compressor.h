#pragma once


#include <stdint.h>


namespace drc01
{


class Compressor
{
public:
    Compressor(uint8_t *dst, const uint8_t *src);
    uint16_t compress();

private:
    bool atEnd() const { return src_ == src_end_; }
    uint16_t countZeros(const uint8_t *at) const;
    uint16_t countNonZeros(const uint8_t *at) const;

    void codeByte(uint8_t val) { *dst_++ = val; }
    void codeNumber(uint16_t num);
    void copyData(uint16_t num);

    uint8_t * const dst_start_;
    uint8_t *       dst_;
    const uint8_t * src_;
    const uint8_t * src_end_;
};


}
