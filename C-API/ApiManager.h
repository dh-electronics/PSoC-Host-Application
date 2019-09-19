#pragma once


#include <IApiManager.h>
#include "resultcodes.h"
#include "leds.h"
#include <peripherals/Led.h>
#include <peripherals/Buttons.h>
#include <peripherals/Display.h>
#include <spi/SpiProto.h>
#include <hal/System.h>
#include <hal/GPIO.h>
#include <signal.h>


namespace drc01
{


class ApiManager : public IApiManager
{
public:
    static RESULT openApi();
    static RESULT closeApi();
    static ApiManager * self() { return self_; }

    virtual void            active(bool active = true);         // used for detecting client access to api
    virtual ILed *          getLed(LED led) const;
    virtual IButtons *      getButtons() const                  { return buttons_; }
    virtual IDisplay *      getDisplay() const                  { return display_; }

    virtual void            resetStats()                        { proto_.resetStats(); }
    virtual unsigned int    getCrcErrorCounter() const          { return proto_.getCrcErrorCounter(); }
    virtual unsigned int    getResponseErrorCounter() const     { return proto_.getResponseErrorCounter(); }
    virtual unsigned int    getBusyCounter() const              { return proto_.getBusyCounter(); }
    virtual unsigned int    getTimeoutsCounter() const          { return proto_.getTimeoutsCounter(); }
    virtual unsigned int    getResetsCounter() const            { return proto_.getResetsCounter(); }
    virtual unsigned int    getPicVersion() const               { return uint32_t(picVersion_); }
    virtual RESULT          readPicVersion();
    virtual unsigned int    getHwRevision() const               { return hwRevision_; }

private:
    static const int64_t LOG_STATS_PERIOD_uS = 1000000ULL * 3600 * 24; // every 24 hours
    static const int32_t READY_LED_BLINK_PERIOD_mS = 150;   // 150 mS
    static const uint16_t PASSIVE_MODE_WAITTIME = 10;       // 10 mS
    static const int32_t RESET_PULSE_PERIOD_mS = 2;         // 2 mS
    static const int32_t BOOTLOADER_WAIT_PERIOD_mS = 1250;  // 1250 mS

    enum PIC_SW
    {
        PIC_SW_0V1 = 0x0001,     // v.0.1
    };

    enum HW_REV
    {
        HW_REV_100 = 100,
        HW_REV_200 = 200,
        HW_REV_300 = 300,
        HW_REV_400 = 400,
        HW_REV_500 = 500
    };

    ApiManager();
    virtual ~ApiManager();

    bool        start();
    bool        createPeripherals();
    void        deletePeripherals();

    void        resetPic();
    bool        detectCpu();
    bool        readHwRevision();
    bool        writePicHwRevision();
    bool        writePicMode(bool active);
    bool	    readPicMode();
    void        stateRestore();

    int32_t     periodicActions();
    SpiProto &  proto() { return proto_; }

    static void sigactionHandler(int sig);

    SpiProto                                 proto_;
    Led *                                    leds_[Led::COUNT];
    Buttons *                                buttons_;
    Display *                                display_;
    int16_t                                  picVersion_;
    int16_t                                  hwRevision_;
    dhcom::System                            system_;
    std::chrono::steady_clock::time_point    readyLedBlinkTimePoint_;
    std::chrono::steady_clock::time_point    logStatsTimePoint_;
    bool            active_;

    static ApiManager * self_;

    friend class SpiProto;
};


}
