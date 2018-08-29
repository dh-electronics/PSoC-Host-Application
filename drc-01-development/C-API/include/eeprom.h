#pragma once



#include "resultcodes.h"



enum RESULT writeEeprom(unsigned short *data, unsigned short offset, unsigned char wordCount);
