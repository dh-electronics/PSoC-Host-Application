#ifndef BUTTONS_H_
#define BUTTONS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "resultcodes.h"


/// Represents the PCAP BUttons states.
struct BUTTONS
{
    char esc:1, //< button Esc was pressed
        dn:1,   //< button Down was pressed
        m:1,    //< button Middle was pressed
        ok:1,   //< button Ok was pressed
        up:1;   //< button Up was pressed
};


struct BUTTONS readButtons(enum RESULT *result);


struct BUTTONS getButtons(enum RESULT *result);


#ifdef __cplusplus
}
#endif

#endif // BUTTONS_H_
