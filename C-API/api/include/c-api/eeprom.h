#pragma once

/**
 * \file eeprom.h
 * \brief Access to permanent memory connected to the PIC
 *
 * \attention Only at BigFish.
 */

#include "resultcodes.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief write data to permanent memory
 *
 * \attention Uncertain! Configuration parameters such as HW Revision are also
 * stored in the EEPROM and with this function call the parameters can be
 * destroyed. The only parameter for writing is a splash bitmap, which is
 * located at address 0 and has a length of up to 512 bytes. Splash bitmap must
 * be specially compressed before it can be written to memory.
 *
 * \param data point to datawords buffer
 * \param offset address offset to write to
 * \param wordCount count of words to write
 *
 * \return result code of requested operation
 */
enum RESULT writeEeprom(unsigned short *data, unsigned short offset, unsigned char wordCount);


#ifdef __cplusplus
}
#endif

