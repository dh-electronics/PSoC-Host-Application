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


RESULT displayFill(bool white)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->fill(white);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displayFillRect(int x, int y, int w, int h, bool white)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->fillRect(x, y, w, h, white);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}



RESULT displayDrawRect(int x, int y, int w, int h, bool white)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->drawRect(x, y, w, h, white);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}



RESULT displayInvertRect(int x, int y, int w, int h)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->invertRect(x, y, w, h);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}



RESULT displayBitmap(int x, int y, const Bitmap &bmp)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->bitmap(x, y, bmp);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}



RESULT displayBitmap(int x, int y, const FT_Bitmap &bmp)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->bitmap(x, y, bmp);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displayFlush()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->flush();
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displaySwap()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->swap();
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}
