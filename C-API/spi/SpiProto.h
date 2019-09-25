#pragma once


#include "peripherals/Gpios.h"
#include "resultcodes.h"
#include <spi/MasterHelpers.h>
#include <hal/SPI.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <pthread.h>
#include <stdint.h>


namespace drc01
{

class SpiProto
{
public:
    static const uint32_t           SPI_FREQ        = 500000;  // 500KHz
    static const uint32_t           CMD_REPEATS     = 5;        // Times
    static const uint32_t           RSP_REPEATS     = 5;        // Times
    static const int32_t            HEARTBEAT_MS    = 1000;     // MS
    static const uint32_t           BUSY_WAIT_MS    = 3;        // MS
    static const uint32_t           CMD_WAIT_MS     = 1;        // 1 MS, standard waittime
    static const uint8_t            DLC_PACKET_SIZE = SPI_PACKET_SIZE * 2 + 1; // taking into account the ESC symbols and the DEL at end of the packet
    static const uint8_t            SYM_ESC         = 0x1B;
    static const uint8_t            SYM_DEL         = 0x7F;

    SpiProto();
    ~SpiProto();

    void        resetStats();
    uint32_t    getCrcErrorCounter() const;
    uint32_t    getResponseErrorCounter() const;
    uint32_t    getBusyCounter() const;
    uint32_t    getTimeoutsCounter() const;
    uint32_t    getResetsCounter() const;

    bool init(const dhcom::System &sys);
    bool start();
    void stop();

    // commonly used services
    RESULT      gpioControl(GPIO gpio, bool on);
    PIC_MODE    readMode(RESULT *result);
    bool        ping(RESULT *result);

    RESULT xmit(GenericCommand *command, uint8_t cmdSize, GenericResponse *response, uint8_t rspSize, uint16_t waitTime);
    template <uint8_t csize, uint8_t rsize>
    RESULT xmit(Command <csize> &cmd, Response <rsize> &rsp, uint16_t waitTime = CMD_WAIT_MS)
    {
        return xmit(cmd.cmd(), cmd.getSize(), rsp.rsp(), rsp.getSize(), waitTime);
    }

private:
    static void * pthread_start_routine(void *);
    static uint8_t codeDlcPacket(uint8_t *dest, const uint8_t* src, uint8_t srcSize);

    void run();
    bool isRunning() const;
    void incCrcErrors();
    void incResponseErrors();
    void incBusy();
    void incTimeouts();
    void incResets();


#ifndef DUMMY_PERIPHERALS
    dhcom::SPI *                                spi_;

    pthread_t                                   pthread_;
    mutable std::mutex                          statsMutex_;
    mutable std::mutex                          spiMutex_;
    std::condition_variable                     waitCondition_;

    uint32_t                                    crcErrorCounter_;
    uint32_t                                    responseErrorCounter_;
    uint32_t                                    busyCounter_;
    uint32_t                                    timeoutsCounter_;
    uint32_t                                    resetsCounter_;
    std::chrono::steady_clock::time_point       lastCommTimePoint_;
    uint16_t                                    shiftRegisterData_;
    bool                                        stateRestorePending_;
    uint8_t                                     pingCounter_;
    volatile bool                               running_;
#endif
};


}
