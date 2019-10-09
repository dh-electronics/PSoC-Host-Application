#include <c-api/buttons.h>
#include <c-api/resultcodes.h>
#include "IApiManager.h"
#include "peripherals/Buttons.h"


using namespace drc01;


enum RESULT setButtonsCallback(buttonsCallback *callback)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->getButtons()->setCallback(callback);
        return RESULT_OK;
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


enum RESULT handleButtons()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        return man->getButtons()->handleEvents();
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


bool getButtonState(enum BUTTON button, enum RESULT *res)
{
    enum RESULT result;
    bool state;
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        state = man->getButtons()->getState(button);
        result = RESULT_OK;
    }
    else
    {
        state = false;
        result = RESULT_API_NOT_OPEN;
    }

    if(res)
        *res = result;

    return state;
}
