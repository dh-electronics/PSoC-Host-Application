#include "ServiceListener.h"
#include "JsonServer.h"
#include "init.h"
#include <Poco/Net/NetException.h>
#include <iostream>
#include <new>


using namespace Poco;
using namespace Poco::Net;
using namespace bigfish;
using namespace std;


ServiceListener::ServiceListener(const SocketAddress &address, int backLog)
    : address_(address)
    , backlog_((backLog > MAX_BACKLOG) ? MAX_BACKLOG : backLog)
    , shouldStop_(false)
{
}


ServiceListener::~ServiceListener()
{
    stopRunningServers();
    cleanupFinishedServers();
}


int ServiceListener::exec()
{
    // create a server socket
    try
    {
        serverSocket_ = ServerSocket(address_, backlog_);
        printf ("Created server socket\n"); fflush (stdout);

        while(true)
        {
            cleanupFinishedServers();

            bool canGetMore;
            {
                ScopedLock <FastMutex> lock(accessMutex_);
                if(shouldStop_)
                    break;
                canGetMore = runningServers_.size() < size_t(backlog_);
            }

            if(!canGetMore)
            {   // means more servers than backlog_ are running
                usleep(100000); // 100ms
                continue;
            }

            // check if new clients are waiting
            if(!serverSocket_.poll(SERVER_POLL_PERIOD * 1000, Socket::SELECT_READ))
                continue;

            StreamSocket socket = serverSocket_.acceptConnection();

            // insert the new JsonServer
            JsonServer *newServer = new(nothrow) JsonServer(this, socket);
            if(newServer)
            {
                if(newServer->start())
                {
                    ScopedLock <FastMutex> lock(accessMutex_);
                    runningServers_.push_back(newServer);
                }
                else
                {
                    socket.close();
                    delete newServer;
                }
            }
            else
            {
                socket.close();
                cerr << "Could not create a JsonServer, new() failed." << endl;
            }
        }

        serverSocket_.close();
        return EXIT_SUCCESS;
    }
    catch(NetException &ex)
    {
        cerr << "NetException: " << ex.what() << endl;
    }
    catch(exception &ex)
    {
        cerr << "C++ Exception: " << ex.what() << endl;
    }
    catch(...)
    {
        cerr << "Unknown exception." << endl;
        return EXIT_FAILURE;
    }
    serverSocket_.close();
    return EXIT_FAILURE;
}


void ServiceListener::stop()
{
    ScopedLock <FastMutex> lock(accessMutex_);
    shouldStop_ = true;
}


void ServiceListener::jsonServerFinished(JsonServer *server)
{
    ScopedLock <FastMutex> lock(accessMutex_);
    runningServers_.remove(server);
    finishedServers_.push_back(server);
    if(runningServers_.empty())
        idleApi();
}


void ServiceListener::cleanupFinishedServers()
{
    while(true)
    {
        JsonServer *server = NULL;
        {
            ScopedLock <FastMutex> lock(accessMutex_);

            if(finishedServers_.empty())
                break;

            server = finishedServers_.front();
            finishedServers_.pop_front();
        }
        assert(server);
        server->wait();
        delete server;
    }
}


void ServiceListener::stopRunningServers()
{
    {
        ScopedLock <FastMutex> lock(accessMutex_);
        JsonServerList::iterator si = runningServers_.begin(), ei = runningServers_.end();
        for(; si != ei; ++si)
            (*si)->stop();
    }

    // wait until they all move to the finished
    while(true)
    {
        {
            ScopedLock <FastMutex> lock(accessMutex_);
            if(runningServers_.empty())
                break;
        }
        usleep(100000); // idle a little
    }
}
