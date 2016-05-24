#include <stats.h>
#include <resultcodes.h>
#include <peripherals/IApiManager.h>


using namespace drc01;


unsigned int getCrcErrorCounter(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    unsigned int retVal;

    if(man)
    {
        man->active();
        res = RESULT_OK;
        retVal = man->getCrcErrorCounter();
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
        retVal = 0;
    }

    if(result)
        *result = res;

    return retVal;
}


unsigned int getResponseErrorCounter(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    unsigned int retVal;

    if(man)
    {
        man->active();
        res = RESULT_OK;
        retVal = man->getResponseErrorCounter();
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
        retVal = 0;
    }

    if(result)
        *result = res;

    return retVal;
}


unsigned int getBusyCounter(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    unsigned int retVal;

    if(man)
    {
        man->active();
        res = RESULT_OK;
        retVal = man->getBusyCounter();
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
        retVal = 0;
    }

    if(result)
        *result = res;

    return retVal;
}


unsigned int getTimeoutsCounter(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    unsigned int retVal;

    if(man)
    {
        man->active();
        res = RESULT_OK;
        retVal = man->getTimeoutsCounter();
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
        retVal = 0;
    }

    if(result)
        *result = res;

    return retVal;
}


unsigned int getResetsCounter(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    unsigned int retVal;

    if(man)
    {
        man->active();
        res = RESULT_OK;
        retVal = man->getResetsCounter();
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
        retVal = 0;
    }

    if(result)
        *result = res;

    return retVal;
}


void resetStats(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;

    if(man)
    {
        man->active();
        man->resetStats();
        res = RESULT_OK;
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
    }

    if(result)
        *result = res;
}


unsigned int getApiVersion(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;

    if(man)
    {
        man->active();
        res = RESULT_OK;
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
    }

    if(result)
        *result = res;

    return API_VERSION;
}


unsigned int getPicVersion(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    unsigned int retVal;

    if(man)
    {
        man->active();
        retVal = man->getPicVersion();
        res = RESULT_OK;
    }
    else
    {
        retVal = 0;
        res = RESULT_API_NOT_OPEN;
    }

    if(result)
        *result = res;

    return retVal;
}


unsigned int getHwRevision(enum RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    unsigned int retVal;

    if(man)
    {
        man->active();
        retVal = man->getHwRevision();
        res = RESULT_OK;
    }
    else
    {
        retVal = 0;
        res = RESULT_API_NOT_OPEN;
    }

    if(result)
        *result = res;

    return retVal;
}
