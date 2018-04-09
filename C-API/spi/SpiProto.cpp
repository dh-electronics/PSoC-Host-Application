#include "spi/SpiProto.h"
#include "ApiManager.h"
#include <Poco/ScopedLock.h>
#include <Poco/Timestamp.h>
#include <string.h>
#include <cassert>
#include <syslog.h>
#include <unistd.h>


using namespace drc01;
using namespace Poco;
using namespace dhcom;
using namespace std;


SpiProto::SpiProto()
    : spi_(NULL)
    , shiftRegisterData_(0)
    , stateRestorePending_(false)
    , pingCounter_(0)
{
    resetStats();
}


SpiProto::~SpiProto()
{
    stop();
    delete spi_;
}


void SpiProto::resetStats()
{
    ScopedLock <FastMutex> lock(statsMutex_);
    crcErrorCounter_ = responseErrorCounter_ = busyCounter_ = timeoutsCounter_ = resetsCounter_ = 0;
}


uint32_t SpiProto::getCrcErrorCounter() const
{
    ScopedLock <FastMutex> lock(statsMutex_);
    return crcErrorCounter_;
}


uint32_t SpiProto::getResponseErrorCounter() const
{
    ScopedLock <FastMutex> lock(statsMutex_);
    return responseErrorCounter_;
}


uint32_t SpiProto::getBusyCounter() const
{
    ScopedLock <FastMutex> lock(statsMutex_);
    return busyCounter_;
}


uint32_t SpiProto::getTimeoutsCounter() const
{
    ScopedLock <FastMutex> lock(statsMutex_);
    return timeoutsCounter_;
}


uint32_t SpiProto::getResetsCounter() const
{
    ScopedLock <FastMutex> lock(statsMutex_);
    return resetsCounter_;
}


bool SpiProto::init(const dhcom::System &sys)
{
    // create the spi interface
    spi_ = new(nothrow) SPI(sys, SPI::DEVICE_SPI1);
    if(!spi_)
    {
        syslog(LOG_ERR, "Cannot create SPI device.");
        return false;
    }

    do  // false loop
    {
        // open spi interface
        STATUS status = spi_->open();
        if(status != STATUS_SUCCESS)
        {
            syslog(LOG_ERR, "Cannot open the SPI device, status: %x", int(status));
            break;
        }

        // printf("spi params");fflush(stdout);
        status = spi_->setCommParams(SPI::MODE_0, 8, SPI_FREQ);
        if(status != STATUS_SUCCESS)
        {
            syslog(LOG_ERR, "Cannot set SPI comm params, status: %x", int(status));
            break;
        }
        return true;
    }
    while(false);

    spi_->close();
    delete spi_;
    spi_ = NULL;
    return false;
}


bool SpiProto::start()
{
    // start heartbeats
    lastCommTimestamp_.update();
    return 0 == pthread_create(&pthread_, NULL, pthread_start_routine, this);
}


void SpiProto::stop()
{
    running_ = false;
    pthread_join(pthread_, NULL);
}


bool SpiProto::isRunning() const
{
    return running_;
}


RESULT SpiProto::gpioControl(GPIO gpio, bool on)
{
    Command <2> cmd(CMD_GPIO_WRITE);
    set_8(cmd.data(), 0, uint8_t(gpio));
    set_8(cmd.data(), 1, on ? 1 : 0);
    Response <0> rsp;
    return xmit(cmd, rsp);
}


PIC_MODE SpiProto::readMode(RESULT *result)
{
    Command <0> cmd(CMD_MODE_READ);
    Response <1> rsp;
    const RESULT res = xmit(cmd, rsp);
    PIC_MODE mode = PIC_MODE_WAITING;
    if(res == RESULT_OK && get_8(rsp.data(), 0))
        mode = PIC_MODE_ACTIVE;

    if(result)
        *result = res;

    return mode;
}


bool SpiProto::ping(RESULT *result)
{
    Command <1> cmd(CMD_PING);
    *cmd.data() = pingCounter_;
    Response <1> rsp;
    const RESULT res = xmit(cmd, rsp);

    bool retVal = res == RESULT_OK && get_8(rsp.data(), 0) == pingCounter_;
    ++pingCounter_;

    if(result)
        *result = res;

    return retVal;
}


RESULT SpiProto::xmit(GenericCommand *command, uint8_t cmdSize, GenericResponse *response, uint8_t rspSize, uint16_t waitTime)
{
    assert(command);
    assert(cmdSize <= SPI_PACKET_SIZE);
    assert(response);
    assert(rspSize <= SPI_PACKET_SIZE);

    //    Timestamp xmitMeasurement;
    //    xmitMeasurement.update();

    // cout << "csize " << int(cmdSize) << " rsize " << int(rspSize) << endl;

    const uint32_t waitTime_uS = uint32_t(waitTime) * 1000;

    // setting the command's message counter and crc
    cmdCalcCrc(command, cmdSize);

    ScopedLock <FastMutex> lock(spiMutex_); // lock until the whole transaction completes or fails

    // sending the command
    for(uint8_t cmd_attempt = 0; cmd_attempt < CMD_REPEATS; ++cmd_attempt)
    {
        {
            // sending the command
            STATUS status;
            uint8_t rspBytes[DLC_PACKET_SIZE];
            uint8_t sndBytes[DLC_PACKET_SIZE];
            const uint8_t length = codeDlcPacket(sndBytes, reinterpret_cast <uint8_t *> (command), cmdSize);
            spi_->transceive(sndBytes, rspBytes, length, &status);
            if(STATUS_SUCCESS != status)
            {
                syslog(LOG_WARNING, "SPI command sending failed, errno: %s", strerror(errno));
                continue;
            }
        }

        // wait for the PIC to complete
        usleep(waitTime_uS);

        // polling the response
        for(uint8_t rsp_attempt = 0; rsp_attempt < RSP_REPEATS; ++rsp_attempt)
        {
            STATUS status;
            uint8_t rspBytes[DLC_PACKET_SIZE];
            memset(rspBytes, 0, rspSize);
            rspBytes[rspSize] = SYM_DEL;
            spi_->transceive(rspBytes, rspBytes, rspSize + 1, &status);
            if(STATUS_SUCCESS != status)
            {
                syslog(LOG_WARNING, "SPI response receiving failed.");
                continue;
            }
            memcpy(response, rspBytes, rspSize);

            if(!rspCheckCrc(response)) // error in transmission - repeat poll
            {
                incCrcErrors();
                continue;
            }

            if(rspIsBusy(response))
            {
                //                fprintf(stderr, "Busy, cmd: %hhx\n", command->command_);
                // wait and repeat
                incBusy();
                usleep(BUSY_WAIT_MS * 1000);
                continue;
            }

            lastCommTimestamp_.update();

            if(rspIsError(response))
            {
                syslog(LOG_WARNING, "SPI error in response, command: %hhd, code: %x", command->command_, rspErrorCode(response));
                switch(rspErrorCode(response))
                {
                case ERR_MODE:
                    syslog(LOG_ERR, "SPI wrong UI mode reported...");
                    incResets();
                    stateRestorePending_ = true;
                    waitCondition_.signal();
                    return RESULT_COMM_TIMEOUT;

                case ERR_CRC:
                    incCrcErrors();
                    break;

                case ERR_ARG:
                    incResponseErrors();
                    syslog(LOG_ERR, "SPI wrong command argument reported.");
                    return RESULT_ARGUMENT_INVALID;

                case ERR_UNSUPPORTED:
                case ERR_STATE:
                    incResponseErrors();
                    syslog(LOG_ERR, "SPI unsupported command reported.");
                    return RESULT_SERVICE_INVALID;

                default:
                    incResponseErrors();
                    syslog(LOG_ERR, "SPI unknown error reported.");
                    return RESULT_FUNCTION_INVALID;
                }

                // break the rsp_attempt loop so the cmd_attempt can be repeated
                break;
            }

            // printf("\nSPI XFER time: %d", int(xmitMeasurement.elapsed()/1000));
            return RESULT_OK;
        } // rsp_attempt
    } // cmd_attempt

    incTimeouts();
    syslog(LOG_ERR, "SPI Communication lost...");
    if(ApiManager::self())
    {
        static_cast <ApiManager*> (ApiManager::self())->resetPic();
    }
    return RESULT_COMM_TIMEOUT;
}


void * SpiProto::pthread_start_routine(void *arg)
{
    SpiProto *self = reinterpret_cast <SpiProto *> (arg);
    assert(self);
    self->run();
    return NULL;
}


uint8_t SpiProto::codeDlcPacket(uint8_t *dest, const uint8_t* src, uint8_t srcSize)
{
    uint8_t * dst = dest;
    const uint8_t * const src_end = src + srcSize;
    for(; src != src_end; ++src)
    {
        uint8_t sym = *src;
        switch(sym)
        {
        case SYM_ESC:
        case SYM_DEL:
            *dst++ = SYM_ESC;
        }
        *dst++ = sym;
    }

    *dst++ = SYM_DEL;
    return dst - dest;
}


void SpiProto::run()
{
    running_ = true;

    while(isRunning())
    {
        int32_t msToNextPeriodic = ApiManager::self()->periodicActions();

        bool heartBeat = false;
        {
            ScopedLock <FastMutex> lock(spiMutex_);
            if(stateRestorePending_)
            {
                stateRestorePending_ = false;
                heartBeat  = false;
            }
            else
            {
                int32_t msToHeartbeat = HEARTBEAT_MS - lastCommTimestamp_.elapsed() / 1000;
                if(msToHeartbeat > 0)
                {
                    try
                    {
                        waitCondition_.wait(spiMutex_, (msToHeartbeat < msToNextPeriodic) ? msToHeartbeat : msToNextPeriodic);
                    }
                    catch(TimeoutException &)
                    {
                    }
                    continue;
                }
                else
                {
                    heartBeat = true;
                }
            }
        }

        if(heartBeat)
        {
            RESULT res;
            bool pingResult = ping(&res);
            if(RESULT_OK == res)
            {
                if(pingResult)
                {
                    PIC_MODE mode = readMode(&res);
                    if(RESULT_OK == res && mode != PIC_MODE_ACTIVE)
                        ApiManager::self()->stateRestore();
                }
                else
                {
                    syslog(LOG_WARNING, "SPI Ping failed...");
                    incResets();

                    ApiManager::self()->resetPic();
                    ApiManager::self()->stateRestore();
                }
            }
        }
        else
        {
            ApiManager::self()->stateRestore();
        }
    }
}


void SpiProto::incCrcErrors()
{
    ScopedLock <FastMutex> lock(statsMutex_);
    ++crcErrorCounter_;
}


void SpiProto::incResponseErrors()
{
    ScopedLock <FastMutex> lock(statsMutex_);
    ++responseErrorCounter_;
}


void SpiProto::incBusy()
{
    ScopedLock <FastMutex> lock(statsMutex_);
    ++busyCounter_;
}


void SpiProto::incTimeouts()
{
    ScopedLock <FastMutex> lock(statsMutex_);
    ++timeoutsCounter_;
}


void SpiProto::incResets()
{
    ScopedLock <FastMutex> lock(statsMutex_);
    ++resetsCounter_;
}



