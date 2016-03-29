#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "resultcodes.h"


enum RESULT displayEnable(int on);


enum RESULT displayClear();


enum RESULT displayInvert(int on);


enum RESULT displaySelectFont(int font);


enum RESULT displaySetPos(int x, int y);


enum RESULT displayText(char *text);


enum RESULT displayShow();


#ifdef __cplusplus
}
#endif

#endif // DISPLAY_H_
