/* SPDX-License-Identifier: BSD-3-Clause */
/* SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH */

#include <c-api/bitmap.h>
#include <c-api/display.h>
#include <IApiManager.h>
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


RESULT displaySetContrast(int value)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->setContrast(value);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displaySetDimTimeout(short value)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->setDimTimeout(value);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displaySetOffTimeout(short value)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->setOffTimeout(value);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}

RESULT displaySetOLedSaver(int enable)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->setOLedSaver(enable);
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
        display->fill(white);
        return RESULT_OK;
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
        display->fillRect(x, y, w, h, white);
        return RESULT_OK;
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
        display->drawRect(x, y, w, h, white);
        return RESULT_OK;
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
        display->invertRect(x, y, w, h);
        return RESULT_OK;
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}



RESULT displayBitmap(int x, int y, const Bitmap *bmp)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        display->bitmap(x, y, *bmp);
        return RESULT_OK;
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}

RESULT displayBitmap2(int x, int y, const FT_Bitmap_ *bmp)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        display->bitmap(x, y, *bmp);
        return RESULT_OK;
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}

RESULT displayScreen(const unsigned char *screen_buffer)
{
    Bitmap bmp(DISPLAY_DEFAULT_WIDTH, DISPLAY_DEFAULT_HEIGHT, screen_buffer);

    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        display->bitmap(0, 0, bmp);
        return RESULT_OK;
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


RESULT displayWriteSplash()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->writeSplash();
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


RESULT displayTakeScreenshot()
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IDisplay *display = man->getDisplay();
        assert(display);
        return display->screenshot();
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}
