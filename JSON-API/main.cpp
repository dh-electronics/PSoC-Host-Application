#include <stdint.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/ServerSocket.h>
#include "ServiceListener.h"
#include "init.h"
#include "helpers.h"
#include <pthread.h>
#include <string.h>

using namespace bigfish;
using namespace Poco;
using namespace Poco::Net;
using namespace std;



ServiceListener *listener = NULL;



static bool parseAddress(const char *arg, IPAddress &ipAddress)
{
    struct in_addr addr;
    if(inet_aton(arg, &addr))
    {
        ipAddress =  IPAddress(&addr, sizeof(addr));
        return true;
    }
    else
    {
        cerr << "Cannot parse adress argument: " << arg << endl;
        return false;
    }
}


static bool parseInt(const char *arg, UInt16 &tcpPort)
{
    uint32_t port = strtoul(arg, NULL, 10);
    if(port < 0x10000)
    {
        tcpPort = UInt16(port);
        return true;
    }
    else
    {
        cerr << "Wrong value for the port argument: " << arg << endl;
        return false;
    }
}


static void showHelp()
{
    cout << "JSON_SERVER\n\n"
            "Usage:\n"
            "\t-a ip_address, default = all available addresses\n"
            "\t-p tcp_port, default = 56882\n"
            "\t-b backlog (simultaneous connections allowed), default = 4" << endl;
}


void sigHandler(int)
{
    if(listener)
        listener->stop();
}


int main(int argc, char **argv)
{
    IPAddress   ipAddress(IPAddress::IPv4);
    UInt16      tcpPort = 56882;    // default
    UInt16      backLog = 4;        // default

    // parse arguments
    while(true)
    {
        int opt = getopt(argc, argv, "a:p:b:h");
        if(opt < 0)
            break;

        switch(opt)
        {
        case 'a': // address spec
            if(!parseAddress(optarg, ipAddress))
                return EXIT_FAILURE;
            break;

        case 'p': // port
            if(!parseInt(optarg, tcpPort))
                return EXIT_FAILURE;
            break;

        case 'b': // backlog
            if(!parseInt(optarg, backLog))
                return EXIT_FAILURE;
            break;

        default:
            showHelp();
            return EXIT_SUCCESS;
        }
    }

    // setting up signals
    {
        struct sigaction sa;
        sigemptyset(&sa.sa_mask);
        sa.sa_handler = sigHandler;
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);
    }

    // open the API
    RESULT res = openApi();
    if(res == RESULT_OK)
    {
        int result = EXIT_FAILURE;
        listener = new(nothrow) ServiceListener(SocketAddress(ipAddress, tcpPort), backLog);
        if(listener)
        {
            cerr << "JSON_SERVER: Starting service\n  IP Address: " << ipAddress.toString() << "\n  TCP port:   " << tcpPort << "\n  Backlog:    " << backLog << endl;
            result = listener->exec();
            delete listener;
            cerr << "JSON_SERVER: Stopped service." << endl;
        }
        closeApi();
        return result;
    }
    else
    {
        cerr << "JSON_SERVER: Error opening the API, code: " << getResultCodeString(res) << endl;
        return EXIT_FAILURE;
    }
}
