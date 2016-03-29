#ifndef EEPROM_API_H
#define EEPROM_API_H


#include "resultcodes.h"


#ifdef __cplusplus
extern "C" {
#endif


enum RESULT writeEeprom(unsigned short *data, unsigned short offset, unsigned char wordCount);


#ifdef __cplusplus
}
#endif

#endif // EEPROM_API_H
