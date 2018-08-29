#include "JsonServer.h"
#include "ServiceListener.h"
#include "ReadStream.h"
#include "WriteStream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "helpers.h"
#include <Poco/Net/NetException.h>
#include <Poco/Timestamp.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>


using namespace Poco;
using namespace Poco::Net;
using namespace drc01;
using namespace rapidjson;
using namespace std;


JsonServer::JsonServer(ServiceListener *listener, Poco::Net::StreamSocket &socket)
    : listener_(listener)
    , socket_(socket)
    , running_(false)
{
    socket_.setLinger(true, 1);
    socket_.setSendTimeout(500);
}


JsonServer::~JsonServer()
{
}


bool JsonServer::start()
{
    return 0 == pthread_create(&pthread_, NULL, pthread_start_routine, this);
}


void JsonServer::wait()
{
    pthread_join(pthread_, NULL);
}


void * JsonServer::pthread_start_routine(void *arg)
{
    JsonServer *self = reinterpret_cast <JsonServer *> (arg);
    assert(self);
    self->run();
    return NULL;
}


void JsonServer::run()
{
    Timestamp parserMeasurement;

    running_ = true;
    ReadStream readStream(socket_);

    while(isRunning())
    {
        jsonState_.clear();

        RESULT result           = RESULT_OK;
        size_t errorPos         = 0;
        const char *errorText   = NULL;
        try
        {
            Reader reader;
            parserMeasurement.update();
            if(!reader.Parse(readStream, jsonState_))
            {
                switch(reader.GetParseErrorCode())
                {
                case kParseErrorDocumentEmpty: // no error, just a timeout
                    cout << "Connection closed." << endl;
                    stop();
                    continue;

                default: // any other parsing errors
                    result = RESULT(-1);
                    errorPos = reader.GetErrorOffset();
                    errorText = "JSON syntax error";
                    stop();
                    break;
                }
            }

            if(result == RESULT_OK)
            {
                // respond
                WriteStream writeStream(socket_);
                jsonState_.handle(writeStream);

                printf("\nJSON Parser time: %d", int(parserMeasurement.elapsed()/1000));

                if(jsonState_.shouldServerShutdown())
                {
                    listener_->stop();
                    stop();
                }
                else if(jsonState_.shouldServerDisconnect())
                {
                    stop();
                }
            }
        }
        catch(NetException &ex)
        {
            cout << "Net Exception: " << strerror(ex.code()) << endl;
            stop();
        }
        catch(const char *what)
        {
            result = jsonState_.getResultCode();
            errorPos = readStream.Tell();
            errorText = what;
            stop();
        }

        if(result != RESULT_OK)
        {
            reportError(result, errorText, errorPos);
            cout << "ERROR: " << errorText << " Pos: " << errorPos << endl;
        }
    }

    socket_.close();
    listener_->jsonServerFinished(this);
}


void JsonServer::reportError(RESULT result, const char* what, size_t pos)
{
    WriteStream writeStream(socket_);
    PrettyWriter <WriteStream> writer(writeStream);

    writer.StartObject().String("result");
    const char *resultCodeString = getResultCodeString(result);
    if(resultCodeString)
    {
        // reporting api error
        writer.String(resultCodeString);
    }
    else
    {
        // reporting parsing error
        writer.String("RESULT_JSON_PARSING_ERROR");

        char strPos[32];
        const int chars = snprintf(strPos, sizeof(strPos), "%d", pos);
        if(chars > 0 && chars <= int(sizeof(strPos)))
            writer.String("pos").String(strPos);
    }

    if(what)
        writer.String("what").String(what);

    writer.EndObject();
}


