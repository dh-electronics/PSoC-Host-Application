// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#pragma once


#include <c-api/display.h>



namespace drc01
{


class IDisplay
{
public:
    virtual ~IDisplay() {}

    virtual RESULT  enable(bool on)                                                  =0;
    virtual RESULT  setContrast(int value)                                           =0;
    virtual RESULT  setDimTimeout(short value)                                       =0;
    virtual RESULT  setOffTimeout(short value)                                       =0;
    virtual RESULT  setOLedSaver(int enable)                                         =0;
    virtual void    fill(bool white)                                                 =0;
    virtual void    fillRect(int x, int y, int w, int h, bool white)                 =0;
    virtual void    drawRect(int x, int y, int w, int h, bool white)                 =0;
    virtual void    invertRect(int x, int y, int w, int h)                           =0;

    virtual void    bitmap(int x, int y, const Bitmap &bmp)                          =0;
    virtual void    bitmap(int x, int y, const FT_Bitmap_ &bmp)                      =0;

    virtual RESULT flush()          =0;
    virtual RESULT swap()           =0;
    virtual RESULT writeSplash()    =0;
    virtual RESULT screenshot()     =0;
};


}
