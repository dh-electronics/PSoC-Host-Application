#ifndef INIT_H
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif


#include <resultcodes.h>


enum RESULT openApi();


enum RESULT closeApi();


enum RESULT idleApi();


#ifdef __cplusplus
}
#endif

#endif // INIT_H
