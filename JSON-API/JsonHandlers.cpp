#include "JsonState.h"
#include "JsonServer.h"
#include "c-api.h"
#include <iostream>
#include "WriteStream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "helpers.h"


using namespace drc01;
using namespace rapidjson;
using namespace std;


void JsonState::funcResetStats(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Extra args for resetStatistics";
    }

    resetStats(&resultCode_);
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "resetStatistics returned an error code";
}


void JsonState::funcGetErrorCounters(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Extra args for getErrorCounters";
    }

    uint32_t crcErrors = getCrcErrorCounter(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getCrcErrorCounter returned an error code";

    uint32_t responseErrors = getResponseErrorCounter(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getResponseErrorCounter returned an error code";

    uint32_t busyCounter = getBusyCounter(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getBusyCounter returned an error code";

    uint32_t timeoutsCounter = getTimeoutsCounter(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getTimeoutsCounter returned an error code";

    uint32_t resetsCounter = getResetsCounter(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getResetsCounter returned an error code";

    PrettyWriter <WriteStream> writer(writeStream);
    writer.StartObject().String("result").String(getResultCodeString(RESULT_OK));
    writer.String("crcErrorCounter").Uint(crcErrors);
    writer.String("responseErrorCounter").Uint(responseErrors);
    writer.String("busyCounter").Uint(busyCounter);
    writer.String("timeoutsCounter").Uint(timeoutsCounter);
    writer.String("resetsCounter").Uint(resetsCounter);
    writer.EndObject();
}


void JsonState::funcGetVersions(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Extra request for getVersions";
    }

    uint32_t apiVersion = getApiVersion(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getApiVersion returned an error code";

    uint32_t picVersion = getPicVersion(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getPicVersion returned an error code";

    uint32_t hwRevision = getHwRevision(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "getHwRevision returned an error code";

    PrettyWriter <WriteStream> writer(writeStream);
    writer.StartObject().String("result").String(getResultCodeString(RESULT_OK));
    writer.String("apiVersion").Uint(apiVersion);
    writer.String("picVersion").Uint(picVersion);
    writer.String("hwRevision").Uint(hwRevision);
    writer.EndObject();
}


void JsonState::funcDisplayEnable(WriteStream &writeStream)
{
    if(seenTags_ != (TAG_FUNC | TAG_ON))
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for displayEnable";
    }

    resultCode_ = displayEnable(on_);
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "displayEnable returned an error code";
}


void JsonState::funcDisplayClear(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Extra args for displayClear";
    }

    resultCode_ = displayClear();
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "displayClear returned an error code";
}


void JsonState::funcDisplayInvert(WriteStream &writeStream)
{
    if(seenTags_ != (TAG_FUNC|TAG_ON))
    {
        resultCode_ = RESULT(-1);
        throw "Extra args for displayInvert";
    }

    resultCode_ = displayInvert(on_);
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "displayInvert returned an error code";
}


void JsonState::funcDisplaySetPos(WriteStream &writeStream)
{
    if(seenTags_ != (TAG_FUNC | TAG_X | TAG_Y))
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for displaySetPos";
    }

    resultCode_ = displaySetPos(intVal1_, intVal2_);
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "displaySetPos returned an error code";
}


void JsonState::funcDisplayText(WriteStream &writeStream)
{
    if(seenTags_ != (TAG_FUNC | TAG_STRVAL))
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for displayText";
    }

    resultCode_ = displayText(const_cast <char *> (strVal_.c_str()));
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "displayText returned an error code";
}


void JsonState::funcDisplaySelectFont(WriteStream &writeStream)
{
    if(seenTags_ != (TAG_FUNC | TAG_VAL))
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for displaySelectFont";
    }

    resultCode_ = displaySelectFont(intVal1_);
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "displaySelectFont returned an error code";
}


void JsonState::funcDisplayShow(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Extra args for displayShow";
    }

    resultCode_ = displayShow();
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "displayShow returned an error code";
}


void JsonState::funcReadButtons(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Extra args for readButtons";
    }

    BUTTONS buttons = readButtons(&resultCode_);
    if(resultCode_ != RESULT_OK)
        throw "readButtons returned an error code";

    PrettyWriter <WriteStream> writer(writeStream);
    writer.StartObject().String("result").String(getResultCodeString(RESULT_OK));
    writer.String("up").Bool(buttons.up);
    writer.String("down").Bool(buttons.dn);
    writer.String("esc").Bool(buttons.esc);
    writer.String("middle").Bool(buttons.m);
    writer.String("ok").Bool(buttons.ok);
    writer.EndObject();
}


void JsonState::funcWriteLed(WriteStream &writeStream)
{
    if(seenTags_ != (TAG_FUNC | TAG_OBJECT | TAG_ON))
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for writeLed";
    }

    resultCode_ = writeLed(objectAsLed(), on_);
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "writeLed returned an error code";
}


void JsonState::funcWriteSplash(WriteStream &writeStream)
{
    if(seenTags_ != (TAG_FUNC | TAG_STRVAL))
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for writeSplash";
    }

    size_t len = strVal_.length() + 1;
    uint8_t words = (len >> 1) + (len & 1);
    resultCode_ = writeEeprom((uint16_t *)strVal_.c_str(), 0, words);
    if(resultCode_ == RESULT_OK)
        writeStandardOk(writeStream);
    else
        throw "writeSplash returned an error code";
}


void JsonState::funcDisconnect(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for closeConnection";
    }

    serverShouldDisconnect_ = true;
    writeStandardOk(writeStream);
}


void JsonState::funcShutdown(WriteStream &writeStream)
{
    if(seenTags_ != TAG_FUNC)
    {
        resultCode_ = RESULT(-1);
        throw "Wrong or extra args for funcShutdown";
    }

    serverShouldShutdown_ = true;
    writeStandardOk(writeStream);
}
