#include "buttons.h"
#include "resultcodes.h"
#include "peripherals/IApiManager.h"
#include "peripherals/Buttons.h"


using namespace drc01;


enum RESULT setButtonsCallback(buttonsCallback *callback)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
        return man->getButtons()->setCallback(callback);
    else
        return RESULT_API_NOT_OPEN;
}


enum RESULT handleButtons()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        return man->getButtons()->handleEvents(result);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}
