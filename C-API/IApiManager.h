// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#pragma once

#include <c-api/leds.h>


namespace drc01
{


class ILed;
class IButtons;
class IDisplay;


class IApiManager
{
public:
    static IApiManager *getIface();

    virtual ~IApiManager(){}

    virtual void            active(bool active = true)      =0;
    virtual ILed *          getLed(LED led) const           =0;
    virtual IButtons *      getButtons() const              =0;
    virtual IDisplay *      getDisplay() const              =0;

    virtual void            resetStats()                    =0;
    virtual unsigned int    getCrcErrorCounter() const      =0;
    virtual unsigned int    getResponseErrorCounter() const =0;
    virtual unsigned int    getBusyCounter() const          =0;
    virtual unsigned int    getTimeoutsCounter() const      =0;
    virtual unsigned int    getResetsCounter() const        =0;
    virtual unsigned int    getPicVersion() const           =0;
    virtual RESULT          readPicVersion()                =0;
    virtual unsigned int    getHwRevision() const           =0;
};


}

