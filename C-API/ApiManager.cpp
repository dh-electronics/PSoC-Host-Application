#include "ApiManager.h"
#include "resultcodes.h"
#include "peripherals/Input.h"
#include "peripherals/Relay.h"
#include "peripherals/Dac.h"
#include "peripherals/LedOnGpio.h"
#include "peripherals/LedOnSR.h"
#include "peripherals/Buttons.h"
#include "peripherals/Display.h"
#include "peripherals/Eeprom.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/MasterHelpers.h"
#include <hal/System.h>
#include <stats.h>
#include <string.h>
#include <new>
#include <syslog.h>


using namespace bigfish;
using namespace dhcom;
using namespace std;


ApiManager * ApiManager::self_ = NULL;


ApiManager::ApiManager()
    : sr_(NULL)
    , buttons_(NULL)
    , display_(NULL)
    , eeprom_(NULL)
    , picVersion_(0)
    , hwRevision_(0)
    , active_(false)
{
    for(int i = 0; i < Input::COUNT; ++i)
        inputs_[i] = NULL;

    for(int i = 0; i < Relay::COUNT; ++i)
        relais_[i] = NULL;

    for(int i = 0; i < Dac::COUNT; ++i)
        dacs_[i] = NULL;

    for(int i = 0; i < Led::COUNT; ++i)
        leds_[i] = NULL;
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
    openlog("bigfish", LOG_PERROR, LOG_USER);
    syslog(LOG_INFO, "SW version %hhx.%hhx, Starting...", uint8_t(API_VERSION >> 8), uint8_t(API_VERSION));

    if(!detectCpu())
    {
        syslog(LOG_ERR, "Cannot detect Cpu type. Stopping.");
        return false;
    }

    SPI::CHIPSELECT chipSelect;
    switch(system_.getHardware())
    {
    case System::HARDWARE_DHCOM_AM33:
        chipSelect = SPI::CS_1;
        syslog(LOG_INFO, "Running on AM33 CPU...");
        break;

    case System::HARDWARE_DHCOM_AM35:
        chipSelect = SPI::CS_0;
        syslog(LOG_INFO, "Running on AM35 CPU...");
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

    if(!proto_.init(chipSelect))
    {
        syslog(LOG_ERR, "Cannot initialize SPI protocol. Stopping.");
        return false;
    }

    printf("pic version");fflush(stdout);
    if(readPicVersion() != RESULT_OK)
    {
        syslog(LOG_ERR, "Cannot read PIC sw version. Stopping.");
        return false;
    }

    const uint16_t picVersion = getPicVersion();
    syslog(LOG_INFO, "PIC SW version %hhx.%hhx", uint8_t(picVersion >> 8), uint8_t(picVersion));

    if(picVersion == PIC_SW_1V4)
    {
        // old pic SW is uncapable to run on new HW
        if(getHwRevision() >= HW_REV_200)
        {
            syslog(LOG_ERR, "PIC SW incompatible to HW. Please update PIC. Stopping.");
            return false;
        }
    }
    else if(picVersion >= PIC_SW_2V0)
    {
        if(!writePicHwRevision())
        {
            syslog(LOG_ERR, "Cannot write HW revision to PIC. Stopping.");
            return false;
        }
    }
    else
    {
        syslog(LOG_ERR, "Unknown PIC version. Please update PIC. Stopping.");
        return false;
    }

    printf("write pic mode");fflush(stdout);
    // pic must be in the active mode before peripherals are created
    if(!writePicMode(true))
    {
        syslog(LOG_ERR, "Cannot put PIC into active mode. Stopping.");
        return false;
    }

    if(!createPeripherals())
    {
        syslog(LOG_ERR, "Cannot create peripherals. Stopping.");
        return false;
    }

    // start the periodic tasks
    readyLedBlinkTimestamp_.update();
    logStatsTimestamp_.update();
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
    // creating the Shift Register service - dependent on PIC SW ver
    sr_ = (getPicVersion() == PIC_SW_1V4)
            ? static_cast <SR*> (new(nothrow) SR1v4(proto_))
            : static_cast <SR*> (new(nothrow) SR2v0(proto_));
    if(!sr_)
        return false;

    // creating the inputs
    // input object is dependant on PIC Protocol (PIC SW ver)
    if(getPicVersion() == PIC_SW_1V4)
    {
        inputs_[0] = new(nothrow) Input1v4(proto_, 0, *sr_, 1, 2);
        inputs_[1] = new(nothrow) Input1v4(proto_, 1, *sr_, 3, 4);
        inputs_[2] = new(nothrow) Input1v4(proto_, 2, *sr_, 5, 6);
        inputs_[3] = new(nothrow) Input1v4(proto_, 3, *sr_, 7, 0);
    }
    else
    {
        inputs_[0] = new(nothrow) Input2v0(proto_, 0);
        inputs_[1] = new(nothrow) Input2v0(proto_, 1);
        inputs_[2] = new(nothrow) Input2v0(proto_, 2);
        inputs_[3] = new(nothrow) Input2v0(proto_, 3);
    }
    // checking that all inputs have been created
    for(int i = 0; i < Input::COUNT; ++i)
    {
        if(!inputs_[i])
            return false;
    }

    // creating the relais - not dependant on PIC SW ver or HW rev
    relais_[0] = new(nothrow) Relay(proto_, RA10);
    relais_[1] = new(nothrow) Relay(proto_, RA11);
    for(int i = 0; i < Relay::COUNT; ++i)
    {
        if(!relais_[i])
            return false;
    }

    // creating the dacs - not dependant on PIC SW ver or HW rev
    for(int i = 0; i < Dac::COUNT; ++i)
    {
        Dac *dac = new(nothrow) Dac(proto_, i);
        if(!dac)
            return false;

        dacs_[i] = dac;
    }

    // creating the Leds on GPIO - not dependent on SW ver or HW rev
    leds_[LED_RS232_A]  = new(nothrow) LedOnGpio(proto_, RC1);
    leds_[LED_RS232_B]  = new(nothrow) LedOnGpio(proto_, RC2);
    leds_[LED_CAN]      = new(nothrow) LedOnGpio(proto_, RA8);

    // creating the Leds on SR - pin number dependent on HW rev
    if(getHwRevision() == HW_REV_100)
    {
        leds_[LED_READY]    = new(nothrow) LedOnSR(*sr_, 13);
        leds_[LED_STATUS]   = new(nothrow) LedOnSR(*sr_, 14);
        leds_[LED_ALARM]    = new(nothrow) LedOnSR(*sr_, 12);
        leds_[LED_UPLINK]   = new(nothrow) LedOnSR(*sr_, 15);
    }
    else
    {
        leds_[LED_READY]    = new(nothrow) LedOnSR(*sr_, 2);
        leds_[LED_STATUS]   = new(nothrow) LedOnSR(*sr_, 1);
        leds_[LED_ALARM]    = new(nothrow) LedOnSR(*sr_, 3);
        leds_[LED_UPLINK]   = new(nothrow) LedOnSR(*sr_, 0);
    }

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

    // creating eeprom - not dependent on anything
    eeprom_ = new(nothrow) Eeprom(proto_);
    if(!eeprom_)
        return false;

    return true;
}


void ApiManager::deletePeripherals()
{
    for(int i = 0; i < Input::COUNT; ++i)
        delete inputs_[i];

    for(int i = 0; i < Relay::COUNT; ++i)
        delete relais_[i];

    for(int i = 0; i < Dac::COUNT; ++i)
        delete dacs_[i];

    for(int i = 0; i < Led::COUNT; ++i)
        delete leds_[i];

    delete buttons_;
    delete display_;
    delete eeprom_;
    delete sr_;
}


void ApiManager::active(bool active)
{
    active_ = active;
    if(!active)
        readyLedBlinkTimestamp_.update();
}


IInput * ApiManager::getInput(INPUT input) const
{
    if(input < 0 || input >= Input::COUNT)
        return NULL;
    return inputs_[input];
}


IRelay * ApiManager::getRelay(RELAY relay) const
{
    if(relay < 0 || relay >= Relay::COUNT)
        return NULL;
    return relais_[relay];
}


IDac * ApiManager::getDac(DAC dac) const
{
    if(dac < 0 || dac >= Dac::COUNT)
        return NULL;
    return dacs_[dac];
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
    if(logStatsTimestamp_.elapsed() >= LOG_STATS_PERIOD_uS)
    {
        logStatsTimestamp_.update();
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

    int32_t timeLeft = READY_LED_BLINK_PERIOD_mS - readyLedBlinkTimestamp_.elapsed() / 1000;
    if(timeLeft <= 0)
    {
        readyLedBlinkTimestamp_.update();
        ILed *led = getLed(LED_READY);
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
    syslog(LOG_WARNING, "Have to reset the PIC - counters persistence compromised");

    dhcom::GPIO gpio_E(system_, dhcom::GPIO::PORT_GPIO_E);
    if(gpio_E.open() == STATUS_SUCCESS
            && gpio_E.setDirection(dhcom::GPIO::DIRECTION_OUTPUT) == STATUS_SUCCESS)
    {
        gpio_E.set(false);
        usleep(1000);   // 1ms
        gpio_E.set(true);
        usleep(100000); // 100ms
    }
    else
    {
        syslog(LOG_ERR, "Cannot setup the PIC reset GPIO");
    }
    gpio_E.close();
}

/*

===== Cpuinfo for AM33 =====
processor	: 0
model name	: ARMv7 Processor rev 2 (v7l)
BogoMIPS	: 495.61
Features	: half thumb fastmult vfp edsp thumbee neon vfpv3 tls vfpd32
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x3
CPU part	: 0xc08
CPU revision	: 2

Hardware	: Generic AM33XX (Flattened Device Tree)
Revision	: 0000
Serial		: 0000000000000000

===== Cpuinfo for AM35 =====
Processor	: ARMv7 Processor rev 7 (v7l)
BogoMIPS	: 597.64
Features	: swp half thumb fastmult vfp edsp thumbee neon vfpv3 tls
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x1
CPU part	: 0xc08
CPU revision	: 7

Hardware	: DHCM3517
Revision	: 0020
Serial		: 0000000000000000
*/
bool ApiManager::detectCpu()
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    if(!file)
        return false;

    System::HARDWARE hw = System::HARDWARE_INVALID;
    char *line = NULL;
    size_t size;

    while(getline(&line, &size, file) != -1)
    {
        if(strstr(line, "CPU implementer") && !strstr(line, "0x41"))
            break;

        if(strstr(line, "CPU architecture") && !strstr(line, "7"))
            break;

        if(strstr(line, "CPU variant"))
        {
            if(strstr(line, "0x3"))
            {
                hw = System::HARDWARE_DHCOM_AM33;
                break;
            }
            else if(strstr(line, "0x1"))
            {
                hw = System::HARDWARE_DHCOM_AM35;
                break;
            }
        }
    }

    delete line;
    fclose(file);

    if(hw == System::HARDWARE_INVALID)
        return false;

    system_.setHardware(hw);
    return true;
}


bool ApiManager::readHwRevision()
{
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
    // dump(&rsp, sizeof(rsp));
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
        res = proto_.xmit(cmd, rsp);
    }
    else
    {
        set_8(cmd.data(), 0, PIC_MODE_WAITING);
        res = proto_.xmit(cmd, rsp, PASSIVE_MODE_WAITTIME);
    }

    // dump(&rsp, sizeof(rsp));
    return RESULT_OK == res;
}


void ApiManager::stateRestore()
{
    if(!writePicMode(true))
        return;

    for(int i = 0; i < Input::COUNT; ++i)
        inputs_[i]->restore();

    for(uint8_t i = 0; i < Relay::COUNT; ++i)
        relais_[i]->restore();

    for(uint8_t i = 0; i < Dac::COUNT; ++i)
        dacs_[i]->restore();

    for(uint8_t i = 0; i < Led::COUNT; ++i)
        leds_[i]->restore();
}


IApiManager *IApiManager::getIface()
{
    return ApiManager::self();
}
