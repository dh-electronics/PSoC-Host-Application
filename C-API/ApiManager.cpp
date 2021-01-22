#include "ApiManager.h"
#include "peripherals/LedOnGpio.h"
#include "peripherals/Buttons.h"
#include "peripherals/Display.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/MasterHelpers.h"
#include <hal/System.h>
#include <c-api/stats.h>
#include <string.h>
#include <new>
#include <syslog.h>
#include <unistd.h>
#include <stdio.h>


using namespace drc01;
using namespace dhcom;
using namespace std;


ApiManager * ApiManager::self_ = NULL;


ApiManager::ApiManager()
    : buttons_(NULL)
    , display_(NULL)
    , picVersion_(0)
    , hwRevision_(0)
    , active_(false)
{
    for(int i = 0; i < Led::COUNT; ++i)
        leds_[i] = NULL;

    { // setting up signal handling
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = sigactionHandler;
        sigaction(SIGUSR1, &sa, NULL);
    }
}


ApiManager::~ApiManager()
{
    writePicMode(false);
    proto_.stop();
    deletePeripherals();

    self_ = NULL;
    closelog();
}


bool ApiManager::start()
{
    int retrys = 5;

    openlog("drc-01", LOG_PERROR, LOG_USER);
    syslog(LOG_INFO, "%s SW version %d.%d, Starting...",
    		API_NAME, API_VERSION, API_REVISION);

    System::HARDWARE hw = System::detectSoC();
    system_.setHardware(hw);

    switch(system_.getHardware())
    {
    case System::HARDWARE_DHCOM_AM33:
        syslog(LOG_INFO, "Running on AM33 CPU...");
        break;

    case System::HARDWARE_DHCOM_AM35:
        syslog(LOG_INFO, "Running on AM35 CPU...");
        break;

    case System::HARDWARE_DHCOM_IMX6_REV300:
        syslog(LOG_INFO, "Running on i.MX6 CPU...");
        break;

    case System::HARDWARE_DHCOM_IMX6ULL:
        syslog(LOG_INFO, "Running on i.MX6ULL CPU...");
        break;

    case System::HARDWARE_DHCOM_STM32MP1:
        syslog(LOG_INFO, "Running on STM32MP1 CPU...");
        break;

    default:
        syslog(LOG_ERR, "Unsupported Cpu type. Stopping.");
        return false;
    }

    if(!readHwRevision())
    {
        syslog(LOG_ERR, "Cannot read HW revision pins. Stopping.");
        return false;
    }
    syslog(LOG_INFO, "Running on HW revision %d...", getHwRevision());

    if(!proto_.init(system_))
    {
        syslog(LOG_ERR, "Cannot initialize SPI protocol. Stopping.");
        return false;
    }

    if(readPicVersion() != RESULT_OK)
    {
        syslog(LOG_ERR, "Cannot read UI MCU sw version. Stopping.");
        return false;
    }

    const uint16_t picVersion = getPicVersion();
    syslog(LOG_INFO, "MCU firmware version %hhx.%hhx", uint8_t(picVersion >> 8), uint8_t(picVersion));

    if(!writePicHwRevision())
    {
        syslog(LOG_ERR, "Cannot write HW revision to UI MCU. Stopping.");
        return false;
    }

    // ui must be in the active mode before peripherals are created
    while (true)
    {
        /*
         * rarely it happend that the MCU mode read was not mode ACTIVE.
         * - Increase of sleeps for waiting for the MCU to finish seems not to help here.
         * - Error very hard to reproduce (very rare).
         * - Workaround: add 5 retrys before aborting with error.
         */
        if(!writePicMode(true))
        {
            syslog(LOG_ERR, "Cannot put UI MCU into active mode. Stopping.");
            return false;
        }

        if(!readPicMode())
        {
            if (retrys)
            {
                syslog(LOG_ERR, "UI MCU not in active mode. Retrys left: %d", retrys);
                retrys--;
                continue;
            }
            syslog(LOG_ERR, "UI MCU not in active mode. Stopping.");
            return false;
        }
        break;
    }

    if(!createPeripherals())
    {
        syslog(LOG_ERR, "Cannot create peripherals. Stopping.");
        return false;
    }

    // start the periodic tasks
    logStatsTimePoint_ = readyLedBlinkTimePoint_ = chrono::steady_clock::now();
    if(!proto_.start())
    {
        syslog(LOG_ERR, "Cannot start a thread for periodic tasks for SPI protocol. Stopping.");
        return false;
    }

    return true;
}


RESULT ApiManager::openApi()
{
    if(ApiManager::self())
        return RESULT_OK;

    ApiManager *man = new(nothrow) ApiManager;
    if(man)
    {
        self_ = man;
        if(man->start())
        {
            return RESULT_OK;
        }
        else
        {
            self_ = NULL;
            delete man;
        }
    }
    else
    {
        syslog(LOG_ERR, "Cannot create self. Stopping");
    }

    return RESULT_API_NOT_OPEN;
}


RESULT ApiManager::closeApi()
{
    delete ApiManager::self();
    return RESULT_OK;
}


bool ApiManager::createPeripherals()
{
    // creating the Leds on GPIO - not dependent on SW ver or HW rev
    leds_[LED_RUN]  = new(nothrow) LedOnGpio(proto_, RA8);
    leds_[LED_LAN]  = new(nothrow) LedOnGpio(proto_, RA10);
    leds_[LED_BUS]  = new(nothrow) LedOnGpio(proto_, RA11);
    leds_[LED_ERR]  = new(nothrow) LedOnGpio(proto_, RC1);

    // checking all leds are created
    for(int i = 0; i < Led::COUNT; ++i)
    {
        if(!leds_[i])
            return false;
    }

    // creating buttons - not dependent on anything
    buttons_ = new(nothrow) Buttons(proto_);
    if(!buttons_)
        return false;

    // creating display - not dependent on anything
    display_ = new(nothrow) Display(proto_);
    if(!display_)
        return false;

    return true;
}


void ApiManager::deletePeripherals()
{
    for(int i = 0; i < Led::COUNT; ++i)
        delete leds_[i];

    delete buttons_;
    delete display_;
}


void ApiManager::active(bool active)
{
    active_ = active;
    if(!active)
    	readyLedBlinkTimePoint_ = chrono::steady_clock::now();
}


ILed * ApiManager::getLed(LED led) const
{
    if(led < 0 || led >= Led::COUNT)
        return NULL;
    return leds_[led];
}


RESULT ApiManager::readPicVersion()
{
    Command <0> cmd(CMD_VERSION_READ);
    Response <2> rsp;
    const RESULT res = proto_.xmit(cmd, rsp);
    if(res == RESULT_OK)
        picVersion_ = get_16(rsp.data(), 0);
    else
        picVersion_ = -1;

    return res;
}


int32_t ApiManager::periodicActions()
{
    auto elapsedTimeSinceLastLog = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - logStatsTimePoint_).count();
    if(elapsedTimeSinceLastLog >= LOG_STATS_PERIOD_uS)
    {
        logStatsTimePoint_ = chrono::steady_clock::now();
        if(proto_.getCrcErrorCounter() ||
                proto_.getResponseErrorCounter() ||
                proto_.getBusyCounter() ||
                proto_.getTimeoutsCounter() ||
                proto_.getResetsCounter())
        {
            syslog(LOG_INFO, "Stats since start:");
            if(proto_.getCrcErrorCounter())
                syslog(LOG_INFO, "  Checksum errors: %d", proto_.getCrcErrorCounter());
            if(proto_.getResponseErrorCounter())
                syslog(LOG_INFO, "  Response errors: %d", proto_.getResponseErrorCounter());
            if(proto_.getBusyCounter())
                syslog(LOG_INFO, "  Busy responses : %d", proto_.getBusyCounter());
            if(proto_.getTimeoutsCounter())
                syslog(LOG_INFO, "  SPI Proto Timeouts (crit): %d", proto_.getTimeoutsCounter());
            if(proto_.getResetsCounter())
                syslog(LOG_INFO, "  Spontaneous Resets (crit): %d", proto_.getResetsCounter());
        }
        else
        {
            syslog(LOG_INFO, "Stats since start: No errors.");
        }
    }

    // if inactive, blink the READY Led
    if(active_)
        return 1000;

    auto elapsedTimeSinceLastReadyLedBlink = chrono::duration_cast<chrono::milliseconds>((chrono::steady_clock::now() - readyLedBlinkTimePoint_)).count();
    int32_t timeLeft = READY_LED_BLINK_PERIOD_mS - elapsedTimeSinceLastReadyLedBlink;
    if(timeLeft <= 0)
    {
    	readyLedBlinkTimePoint_ = chrono::steady_clock::now();
        ILed *led = getLed(LED_RUN);
        led->write(!led->get());
        return READY_LED_BLINK_PERIOD_mS;
    }
    else
    {
        return timeLeft;
    }
}


void ApiManager::resetPic()
{
    // Important! When done, persistence is no longer guaranteed
    syslog(LOG_WARNING, "Have to reset the UI MCU");

    dhcom::GPIO gpio_E(system_, dhcom::GPIO::PORT_GPIO_E);
    if(gpio_E.open() == STATUS_SUCCESS
            && gpio_E.setDirection(dhcom::GPIO::DIRECTION_OUTPUT) == STATUS_SUCCESS)
    {
        gpio_E.set(false);
        usleep(RESET_PULSE_PERIOD_mS * 1000);
        gpio_E.set(true);
        usleep(BOOTLOADER_WAIT_PERIOD_mS * 1000);
    }
    else
    {
        syslog(LOG_ERR, "Cannot setup the UI MCU reset GPIO");
    }
    gpio_E.close();
}

bool ApiManager::readHwRevision()
{
    // hw coding of DRC02 baseboard
    const uint8_t GPIOS = 3;
    dhcom::GPIO gpio_L(system_, dhcom::GPIO::PORT_GPIO_L);
    dhcom::GPIO gpio_K(system_, dhcom::GPIO::PORT_GPIO_K);
    dhcom::GPIO gpio_J(system_, dhcom::GPIO::PORT_GPIO_J);
    dhcom::GPIO *gpios[GPIOS] = { &gpio_J, &gpio_K, &gpio_L };

    uint8_t hwCode = 0;
    for(uint8_t i = 0; i < GPIOS; ++i)
    {
        hwCode <<= 1;

        dhcom::GPIO &gpio = *gpios[i];
        if(gpio.open() != STATUS_SUCCESS)
            return false;

        if(gpio.setDirection(dhcom::GPIO::DIRECTION_INPUT) != STATUS_SUCCESS)
            return false;

        STATUS status;
        if(gpio.get(&status))
            hwCode |= 1;
        if(STATUS_SUCCESS != status)
            return false;

        gpio.close();
    }

    switch(hwCode)
    {
    case 0:
        hwRevision_ = HW_REV_100;
        break;
    case 1:
        hwRevision_ = HW_REV_200;
        break;
    case 2:
        hwRevision_ = HW_REV_300;
        break;
    case 3:
        hwRevision_ = HW_REV_400;
        break;
    case 4:
        hwRevision_ = HW_REV_500;
        break;
    default: // unknown HW
        return false;
    }

    return true;
}


bool ApiManager::writePicHwRevision()
{
    Command <2> cmd(CMD_HWREV_WRITE);
    // not to re-program the PIC we consider for PIC the HW 300 is the HW 200
    int16_t writtenRevision = getHwRevision() > uint32_t(HW_REV_200) ? uint32_t(HW_REV_200) : getHwRevision();
    set_16(cmd.data(), 0, writtenRevision);
    Response <0> rsp;
    return RESULT_OK == proto_.xmit(cmd, rsp);
}


bool ApiManager::writePicMode(bool active)
{
    Command <1> cmd(CMD_MODE_WRITE);
    Response <0> rsp;

    RESULT res;
    if(active)
    {
        set_8(cmd.data(), 0, PIC_MODE_ACTIVE);
        res = proto_.xmit(cmd, rsp, ACTIVE_MODE_WAITTIME);
    }
    else
    {
        set_8(cmd.data(), 0, PIC_MODE_WAITING);
        res = proto_.xmit(cmd, rsp, PASSIVE_MODE_WAITTIME);
    }

    // dump(&rsp, sizeof(rsp));
    return RESULT_OK == res;
}


bool ApiManager::readPicMode()
{
	RESULT res;
	PIC_MODE mode = proto_.readMode(&res);

	if(RESULT_OK != res)
	{
	    syslog(LOG_ERR, "readPicMode Res: %d", res);
	}

	return (RESULT_OK == res && mode == PIC_MODE_ACTIVE);
}


void ApiManager::stateRestore()
{
    if(!writePicMode(true))
        return;

    for(uint8_t i = 0; i < Led::COUNT; ++i)
        leds_[i]->restore();

    display_->restore();
}


void ApiManager::sigactionHandler(int)
{
    ApiManager::self()->display_->dump();
}


IApiManager *IApiManager::getIface()
{
    return ApiManager::self();
}


