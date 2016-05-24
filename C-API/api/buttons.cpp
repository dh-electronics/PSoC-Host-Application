#include "buttons.h"
#include "resultcodes.h"
#include "peripherals/IApiManager.h"
#include "peripherals/Buttons.h"


using namespace drc01;


BUTTONS readButtons(RESULT *result)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        return man->getButtons()->read(result);
    }
    else
    {
        if(result)
            *result = RESULT_API_NOT_OPEN;
        return Buttons::ZERO_BUTTONS;
    }
}


BUTTONS getButtons(RESULT *result)
{
    IApiManager *man = IApiManager::getIface();

    RESULT res;
    BUTTONS retVal;

    if(man)
    {
        man->active();
        retVal = man->getButtons()->get();
        res = RESULT_OK;
    }
    else
    {
        retVal = Buttons::ZERO_BUTTONS;
        res = RESULT_API_NOT_OPEN;
    }

    if(result)
        *result = res;
    return retVal;
}
