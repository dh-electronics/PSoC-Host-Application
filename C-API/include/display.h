#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "resultcodes.h"


enum BMP_FUNC { BMP_FUNC_PUT, BMP_FUNC_SLIDEUP, BMP_FUNC_SLIDEDOWN };


enum RESULT displayEnable(int on);


enum RESULT displayClear();


enum RESULT displayInvert(int on);


enum RESULT displaySetPos(int x, int y);


enum RESULT displayBitmap(unsigned char *data, int width, int height, enum BMP_FUNC func);


#ifdef __cplusplus
}
#endif

#endif // DISPLAY_H_
