#pragma once


#include "resultcodes.h"
#include <stddef.h>


#define API_VERSION 0x0006


unsigned int getCrcErrorCounter(enum RESULT *result = NULL);


unsigned int getResponseErrorCounter(enum RESULT *result = NULL);


unsigned int getBusyCounter(enum RESULT *result = NULL);


unsigned int getTimeoutsCounter(enum RESULT *result = NULL);


unsigned int getResetsCounter(enum RESULT *result = NULL);


void resetStats(enum RESULT *result = NULL);


unsigned int getApiVersion(enum RESULT *result = NULL);


unsigned int getPicVersion(enum RESULT *result = NULL);


unsigned int getHwRevision(enum RESULT *result = NULL);

