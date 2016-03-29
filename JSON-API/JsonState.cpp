#include "JsonState.h"
#include "WriteStream.h"
#include "resultcodes.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "helpers.h"
#include <iostream>


using namespace drc01;
using namespace rapidjson;
using namespace std;


JsonState::JsonState()
{
    clear();
}


void JsonState::clear()
{
    seenTags_ = currentTag_ = TAG_NULL;
    handlerFunc_ = NULL;
    resultCode_ = RESULT_OK;
    serverShouldShutdown_ = serverShouldDisconnect_ = false;
}


void JsonState::handle(WriteStream &writeStream)
{
    if(handlerFunc_)
        (this->*handlerFunc_)(writeStream);
    else
    {
        resultCode_ = RESULT(-1);
        throw "Handler function not set";
    }
}


void JsonState::Null()
{
    resultCode_ = RESULT(-1);
    throw "Null not expected here";
}


void JsonState::Bool(bool b)
{
    if(TAG_ON != currentTag_)
    {
        resultCode_ = RESULT(-1);
        throw "Bool not expected here";
    }

    on_ = b;
    endCurrentTag();
}


void JsonState::Int(int i)
{
    switch(currentTag_)
    {
    case TAG_VAL:
    case TAG_X:
        intVal1_ = i;
        break;

    case TAG_Y:
        intVal2_ = i;
        break;

    default:
        resultCode_ = RESULT(-1);
        throw "Int not expected here";
    }
    endCurrentTag();
}


void JsonState::Uint(unsigned i)
{
    Int(i);
}


void JsonState::Int64(int64_t)
{
    resultCode_ = RESULT(-1);
    throw "Int64 not expected here";
}


void JsonState::Uint64(uint64_t)
{
    resultCode_ = RESULT(-1);
    throw "Uint64 not expected here";
}


void JsonState::Double(double)
{
    resultCode_ = RESULT(-1);
    throw "Double not expected here";
}


void JsonState::String(const Ch* str, rapidjson::SizeType length, bool)
{
    switch(currentTag_)
    {
    case TAG_NULL:  // tag name itself
        currentTag_ = tagFromStr(str, length);
        if(TAG_NULL == currentTag_)
        {
            resultCode_ = RESULT(-1);
            throw "Field unexpected";
        }
        return;

    case TAG_FUNC:
        func_ = funcFromStr(str, length);
        if(FUNC_INVALID == func_)
        {
            resultCode_ = RESULT(-1);
            throw "Unknown func";
        }
        break;

    case TAG_OBJECT:
        object_ = objectFromStr(str, length);
        if(OBJECT_INVALID == object_)
        {
            resultCode_ = RESULT(-1);
            throw "Unknown object type";
        }
        break;

    case TAG_VAL:
        currentTag_ = TAG_STRVAL;
        strVal_ = std::string(str, length);
        break;

    default:
        resultCode_ = RESULT(-1);
        throw "String value not expected here";
    }
    endCurrentTag();
}


void JsonState::StartObject()
{
    if(currentTag_ != TAG_NULL)
    {
        resultCode_ = RESULT(-1);
        throw "Object not expected here";
    }
}


void JsonState::EndObject(rapidjson::SizeType)
{
    // end of request - select the handler
    static const HandlerFuncPtr handlers[] = {
        &JsonState::funcResetStats, &JsonState::funcGetErrorCounters,
        &JsonState::funcGetVersions,
        &JsonState::funcWriteInputMode, &JsonState::funcWriteInputThreshold, &JsonState::funcReadInput, &JsonState::funcWriteInputPullup, &JsonState::funcWriteInputShunt,
        &JsonState::funcWriteRelay, &JsonState::funcWatchdog,
        &JsonState::funcWriteDac,
        &JsonState::funcDisplayEnable, &JsonState::funcDisplayClear, &JsonState::funcDisplayInvert,
        &JsonState::funcDisplaySetPos, &JsonState::funcDisplayText,
        &JsonState::funcDisplaySelectFont, &JsonState::funcDisplayShow,
        &JsonState::funcReadButtons,
        &JsonState::funcWriteLed,
        &JsonState::funcWriteSplash,
        &JsonState::funcDisconnect,
        &JsonState::funcShutdown,
    };

    // cout << "In EndObject" << endl;

    if(seenTags_ & TAG_FUNC)
    {
        assert(func_ < FUNC_LAST_FUNC_DUMMY);
        handlerFunc_ = handlers[func_ - 1];
    }
    else
    {
        handlerFunc_ = NULL;
        resultCode_ = RESULT(-1);
        throw "Function is not specified";
    }
}


void JsonState::StartArray()
{
    resultCode_ = RESULT(-1);
    throw "Array not expected here";
}


void JsonState::EndArray(rapidjson::SizeType)
{
    // ignore
}


void JsonState::endCurrentTag()
{
    seenTags_ |= currentTag_;
    currentTag_ = TAG_NULL;
}


JsonState::TAG JsonState::tagFromStr(const char *str, size_t length)
{
    static const int TAGS = 6;
    static const char * const tagNames[TAGS] = {
        "func",     "object",   "on",   "val",      "x",    "y"
    };
    static const TAG tags[TAGS] = {
        TAG_FUNC,   TAG_OBJECT, TAG_ON, TAG_VAL,    TAG_X,  TAG_Y
    };

    for(int i = 0; i < TAGS; ++i)
    {
        if(strncmp(str, tagNames[i], length) == 0)
            return tags[i];
    }
    return TAG_NULL;
}


JsonState::FUNC    JsonState::funcFromStr(const char *str, size_t length)
{
    static const char * const funcNames[FUNC_LAST_FUNC_DUMMY] = {
        "resetStatistics", "getErrorCounters", "getVersions",
        "writeInputMode", "writeInputThreshold",
        "readInput", "writeInputPullup", "writeInputShunt",
        "writeRelay", "watchdog",
        "writeDac",
        "displayEnable", "displayClear", "displayInvert",
        "displaySetPos", "displayText",
        "displaySelectFont", "displayShow",
        "readButtons",
        "writeLed",
        "writeSplash",
        "disconnect",
        "shutdown"
    };

    for(int i = 0; i < FUNC_LAST_FUNC_DUMMY - 1; ++i)
    {
        if(strncmp(str, funcNames[i], length) == 0)
            return FUNC(i + 1);
    }
    return FUNC_INVALID;
}


JsonState::OBJECT    JsonState::objectFromStr(const char *str, size_t length)
{
    static const int OBJECTS = 15;
    static const char * const objectNames[OBJECTS] = {
        "INPUT_1", "INPUT_2", "INPUT_3", "INPUT_4",
        "RELAY_1", "RELAY_2",
        "DAC_1", "DAC_2",
        "LED_RS232_A", "LED_RS232_B",
        "LED_CAN", "LED_READY",
        "LED_STATUS", "LED_ALARM", "LED_UPLINK"
    };

    for(int i = 0; i < OBJECTS; ++i)
    {
        if(strncmp(str, objectNames[i], length) == 0)
            return OBJECT(i + 1);
    }
    return OBJECT_INVALID;
}


void JsonState::writeStandardOk(WriteStream &writeStream)
{
    PrettyWriter <WriteStream> writer(writeStream);
    writer.StartObject().String("result").String(getResultCodeString(RESULT_OK)).EndObject();
}


void JsonState::checkObjectIs(OBJECT minObj, OBJECT maxObj)
{
    if(object_ < minObj || object_ > maxObj)
    {
        resultCode_ = RESULT(-1);
        throw "Wrong value for object field";
    }
}


INPUT JsonState::objectAsInput()
{
    checkObjectIs(OBJECT_INPUT_1, OBJECT_INPUT_4);
    return INPUT(object_ - OBJECT_INPUT_1);
}


RELAY JsonState::objectAsRelay()
{
    checkObjectIs(OBJECT_RELAY_1, OBJECT_RELAY_2);
    return RELAY(object_ - OBJECT_RELAY_1);
}


DAC JsonState::objectAsDac()
{
    checkObjectIs(OBJECT_DAC_1, OBJECT_DAC_2);
    return DAC(object_ - OBJECT_DAC_1);
}


LED JsonState::objectAsLed()
{
    checkObjectIs(OBJECT_LED_RS232_A, OBJECT_LED_UPLINK);
    return LED(object_ - OBJECT_LED_RS232_A);
}
