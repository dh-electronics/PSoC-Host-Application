#ifndef BUTTONS_H_
#define BUTTONS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "resultcodes.h"


struct BUTTONS
{
    char esc:1, dn:1, m:1, ok:1, up:1;
};


struct BUTTONS readButtons(enum RESULT *result);


struct BUTTONS getButtons(enum RESULT *result);


#ifdef __cplusplus
}
#endif

#endif // BUTTONS_H_
