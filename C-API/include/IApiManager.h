#pragma once

#include "resultcodes.h"
#include "inputs.h"
#include "relais.h"
#include "dacs.h"
#include "leds.h"


namespace bigfish
{


class IInput;
class IRelay;
class IDac;
class ILed;
class IButtons;
class IDisplay;
class IEeprom;


class IApiManager
{
public:
    static IApiManager *getIface();

    virtual void            active(bool active = true)      =0;
    virtual IInput *        getInput(INPUT input) const     =0;
    virtual IRelay *        getRelay(RELAY relay) const     =0;
    virtual IDac *          getDac(DAC dac) const           =0;
    virtual ILed *          getLed(LED led) const           =0;
    virtual IButtons *      getButtons() const              =0;
    virtual IDisplay *      getDisplay() const              =0;
    virtual IEeprom *       getEeprom() const               =0;

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


} // namespace bigfish

