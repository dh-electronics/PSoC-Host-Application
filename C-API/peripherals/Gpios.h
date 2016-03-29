#ifndef GPIOS_H
#define GPIOS_H


#ifdef __cplusplus
namespace bigfish
{
#endif


enum GPIO
{
    RA0 = 0x00,
    RA1 = 0x01,
    RA2 = 0x02,
    RA3 = 0x03,
    RA4 = 0x04,     // OLED LDO
    RA5 = 0x05,
    RA6 = 0x06,
    RA7 = 0x07,
    RA8 = 0x08,     // LED_CAN
    RA9 = 0x09,
    RA10 = 0x0A,    // Relay 1
    RA11 = 0x0B,    // Relay 2

    RB0 = 0x10,
    RB1 = 0x11,
    RB2 = 0x12,
    RB3 = 0x13,
    RB4 = 0x14,
    RB5 = 0x15,
    RB6 = 0x16,
    RB7 = 0x17,
    RB8 = 0x18,
    RB9 = 0x19,

    RC0 = 0x20,
    RC1 = 0x21,     // LED_RS232_A
    RC2 = 0x22      // LED_RS232_B
};


#ifdef __cplusplus
}
#endif


#endif // GPIOS_H
