#ifndef COMMAND_H
#define COMMAND_H


#include <stdint.h>
#include <assert.h>
#include <spi/Response.h>


#ifdef __cplusplus
extern "C" {
#pragma pack(1)
#endif


enum CMD
{
    CMD_NULL            = 0x00,
    CMD_MODE_WRITE      = 0x01,
    CMD_GPIO_WRITE      = 0x03,
    CMD_BUTTONS_READ    = 0x08,
    CMD_DISPLAY_FRAME   = 0x0A, // start of frame
    //    0x0B,
    //    0x0C,
    CMD_DISPLAY_WRITE   = 0x0D, // data to display
    CMD_VERSION_READ    = 0x0E,
    CMD_MODE_READ       = 0x0F,
    CMD_HWREV_WRITE     = 0x10, // since protocol v. 2.0

    CMD_PING            = 0x13, // pings the PIC with an argument and returns the same argument

    CMD_SPLASH_ERASE    = 0x14, // erases the Splash in Flash
    CMD_SPLASH_WRITE    = 0x15, // programs the Splash in Flash

    CMD_DISPLAY_CMD         = 0x80, // INTERNAL: up to 14 cbytes data (display command and params) + 2 bytes PIC CMD and CRC
    CMD_DISPLAY_DATA        = 0x81, // INTERNAL: up to 14 cbytes data + 2 bytes PIC CMD and CRC
    CMD_DISPLAY_CONTRAST    = 0x82, // INTERNAL: - controls display contrast
    CMD_DISPLAY_INVERSION   = 0x83, // INTERNAL: - controls display inversion
    CMD_DISPLAY_CHECK       = 0x84, // INTERNAL: Display test pattern
};


enum PIC_MODE
{
    PIC_MODE_WAITING    = 0,
    PIC_MODE_ACTIVE     = 1
};


struct GenericCommand
{
    uint8_t command_;
    uint8_t data_[0];
};


union CommandUnion
{
     struct GenericCommand    cmd;
     uint8_t                  buf[SPI_PACKET_SIZE];
};


#ifdef	__cplusplus
#pragma pack()
}
#endif


#endif // COMMAND_H
