#ifndef STATS_H_
#define STATS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "resultcodes.h"


#define API_VERSION 0x0001


unsigned int getCrcErrorCounter(enum RESULT *result);


unsigned int getResponseErrorCounter(enum RESULT *result);


unsigned int getBusyCounter(enum RESULT *result);


unsigned int getTimeoutsCounter(enum RESULT *result);


unsigned int getResetsCounter(enum RESULT *result);


void resetStats(enum RESULT *result);


unsigned int getApiVersion(enum RESULT *result);


unsigned int getPicVersion(enum RESULT *result);


unsigned int getHwRevision(enum RESULT *result);


#ifdef __cplusplus
}
#endif

#endif // STATS_H_
