#ifndef RESPONSE_H
#define RESPONSE_H


#include <stdint.h>
#include <assert.h>


#ifdef __cplusplus
extern "C" {
#pragma pack(1)
#endif


#define SPI_PACKET_SIZE 16


enum RSP
{
    RSP_OK              = 2,
    RSP_BUSY            = 0xFF, // special response
    RSP_ERR             = 0xE0  // special response
};


enum ERR
{
    ERR_UNSUPPORTED = 0xE0,
    ERR_MODE        = 0xE1,
    ERR_FORMAT      = 0xE2,
    ERR_CRC         = 0xE3,
    ERR_ARG         = 0xE4,
    ERR_STATE       = 0xE5  // since v2.0
};


struct GenericResponse
{
    uint8_t length_;
    uint8_t data_[0];
};


struct SpecialResponse // for busy and null responses
{
    uint8_t code_;
    uint8_t crc_;
};


struct ErrorResponse    // for error responses
{
    uint8_t code_;
    uint8_t crc_;
};


union ResponseUnion
{
     struct GenericResponse   rsp;
     struct SpecialResponse   spc;
     struct ErrorResponse     err;
     uint8_t                  buf[SPI_PACKET_SIZE];
};


#ifdef	__cplusplus
#pragma pack()
}
#endif


#endif // RESPONSE_H
