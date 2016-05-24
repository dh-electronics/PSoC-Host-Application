#include <display.h>
#include <peripherals/IApiManager.h>
#include <peripherals/IDisplay.h>
#include <cassert>


using namespace drc01;


RESULT displayEnable(int on)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->enable(on);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displayClear()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->clear();
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displayInvert(int on)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->invert(on);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displaySelectFont(int font)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->selectFont(font);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displaySetPos(int x, int y)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->setPosition(x, y);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displayBitmap(unsigned char *data, int width, int height, BMP_FUNC func)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->bitmap(data, width, height, func);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}
