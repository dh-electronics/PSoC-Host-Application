#include <resultcodes.h>
#include <IApiManager.h>
#include <ILed.h>
#include <peripherals/Led.h>


using namespace drc01;


RESULT writeLed(LED led, int on)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        ILed *l = man->getLed(led);
        return l ? l->write(on) : RESULT_OBJECT_INVALID;
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


int getLedState(LED led, RESULT *result)
{
    IApiManager *man = IApiManager::getIface();
    RESULT res;
    int retVal;
    if(man)
    {
        man->active();
        ILed *l = man->getLed(led);
        if(l)
        {
            res = RESULT_OK;
            retVal = l->get();
        }
        else
        {
            res = RESULT_OBJECT_INVALID;
            retVal = -1;
        }
    }
    else
    {
        res = RESULT_API_NOT_OPEN;
        retVal = -1;
    }

    if(result)
        *result = res;
    return retVal;
}
