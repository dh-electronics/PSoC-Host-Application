#pragma once


#include <IApiManager.h>
#include "resultcodes.h"
#include "inputs.h"
#include "relais.h"
#include "dacs.h"
#include "leds.h"
#include <peripherals/Input.h>
#include <peripherals/Led.h>
#include <peripherals/Buttons.h>
#include <peripherals/Dac.h>
#include <peripherals/Relay.h>
#include <peripherals/Display.h>
#include <peripherals/Eeprom.h>
#include <peripherals/SR.h>
#include <spi/SpiProto.h>
#include <hal/System.h>
#include <hal/GPIO.h>


namespace bigfish
{


class ApiManager : public IApiManager
{
public:
    static RESULT openApi();
    static RESULT closeApi();
    static ApiManager * self() { return self_; }

    virtual void            active(bool active = true);         // used for detecting client access to api
    virtual IInput *        getInput(INPUT input) const;
    virtual IRelay *        getRelay(RELAY relay) const;
    virtual IDac *          getDac(DAC dac) const;
    virtual ILed *          getLed(LED led) const;
    virtual IButtons *      getButtons() const                  { return buttons_; }
    virtual IDisplay *      getDisplay() const                  { return display_; }
    virtual IEeprom *       getEeprom() const                   { return eeprom_; }

    virtual void            resetStats()                        { proto_.resetStats(); }
    virtual unsigned int    getCrcErrorCounter() const          { return proto_.getCrcErrorCounter(); }
    virtual unsigned int    getResponseErrorCounter() const     { return proto_.getResponseErrorCounter(); }
    virtual unsigned int    getBusyCounter() const              { return proto_.getBusyCounter(); }
    virtual unsigned int    getTimeoutsCounter() const          { return proto_.getTimeoutsCounter(); }
    virtual unsigned int    getResetsCounter() const            { return proto_.getResetsCounter(); }
    virtual unsigned int    getPicVersion() const               { return picVersion_; }
    virtual RESULT          readPicVersion();
    virtual unsigned int    getHwRevision() const               { return hwRevision_; }

private:
    static const int64_t LOG_STATS_PERIOD_uS = 1000000ULL * 3600 * 24; // every 24 hours
    static const int32_t READY_LED_BLINK_PERIOD_mS = 150;   // 150mS
    static const uint16_t PASSIVE_MODE_WAITTIME =
            SpiProto::CMD_WAIT_MS +
            Display::DISPLAY_CLEAR_WAIT_MS +
            Display::DISPLAY_SYMBOL_WAIT_MS * 7 +
            Display::DISPLAY_SHOW_WAIT_MS;

    enum PIC_SW
    {
        PIC_SW_1V4 = 0x0104,     // v.1.4
        PIC_SW_2V0 = 0x0200,     // v.2.0
    };

    enum HW_REV
    {
        HW_REV_100 = 100,
        HW_REV_200 = 200,
        HW_REV_300 = 300
    };

    ApiManager();
    ~ApiManager();

    bool        start();
    bool        createPeripherals();
    void        deletePeripherals();

    void        resetPic();
    bool        detectCpu();
    bool        readHwRevision();
    bool        writePicHwRevision();
    bool        writePicMode(bool active);
    void        stateRestore();

    int32_t     periodicActions();
    SpiProto &  proto() { return proto_; }

    SpiProto        proto_;
    SR *            sr_;
    Input *         inputs_[Input::COUNT];
    Relay *         relais_[Relay::COUNT];
    Dac *           dacs_[Dac::COUNT];
    Led *           leds_[Led::COUNT];
    Buttons *       buttons_;
    Display *       display_;
    Eeprom *        eeprom_;
    int16_t         picVersion_;
    int16_t         hwRevision_;
    dhcom::System   system_;
    Poco::Timestamp readyLedBlinkTimestamp_;
    Poco::Timestamp logStatsTimestamp_;
    bool            active_;

    static ApiManager * self_;

    friend class SpiProto;
};


} // namespace bigfish
