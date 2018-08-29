#pragma once


#include <list>
#include <Poco/Mutex.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/ServerSocket.h>


namespace drc01
{


class JsonServer;


class ServiceListener
{
public:
    static const int MAX_BACKLOG = 8;
    static const int SERVER_POLL_PERIOD = 500; // 500 ms

    ServiceListener(const Poco::Net::SocketAddress &address, int backLog);
    ~ServiceListener();

    int exec();
    void stop();

private:
    void jsonServerFinished(JsonServer *server);
    void cleanupFinishedServers();
    void stopRunningServers();

    const Poco::Net::SocketAddress  address_;
    const int                       backlog_;
    Poco::Net::ServerSocket         serverSocket_;

    Poco::FastMutex                 accessMutex_;
    typedef std::list <JsonServer *> JsonServerList;
    JsonServerList                  runningServers_;
    JsonServerList                  finishedServers_;
    bool                            shouldStop_;

    friend class JsonServer;
};


}
