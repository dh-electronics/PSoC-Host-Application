#pragma once


#include <Poco/Net/StreamSocket.h>
#include <pthread.h>
#include "JsonState.h"


namespace drc01
{


class ServiceListener;


class JsonServer
{
public:
    static const size_t   READ_BUFFER_SIZE = 128;

    JsonServer(ServiceListener *listener, Poco::Net::StreamSocket &socket);
    ~JsonServer();

    bool start();
    void stop()             { running_ = false; }
    void wait();
    bool isRunning() const  { return running_; }

private:
    static const int32_t SOCKET_RECV_TIMEOUT_MS = 500;
    static const int32_t SOCKET_SEND_TIMEOUT_MS = 500;
    static void * pthread_start_routine(void *);

    void run();
    void reportError(RESULT result, const char* what, size_t pos);

    ServiceListener * const         listener_;
    pthread_t                       pthread_;
    Poco::Net::StreamSocket         socket_;
    JsonState                       jsonState_;
    volatile bool                   running_;
};


}
