#pragma once


#include "resultcodes.h"
#include "inputs.h"
#include "relais.h"
#include "dacs.h"
#include "leds.h"
#include <rapidjson/reader.h>
#include <string>


namespace bigfish
{


class WriteStream;


class JsonState : public rapidjson::BaseReaderHandler <>
{
public:
    JsonState();

    void clear();
    void handle(WriteStream &writeStream);
    RESULT getResultCode() const { return resultCode_; }
    bool shouldServerShutdown() const   { return serverShouldShutdown_; }
    bool shouldServerDisconnect() const { return serverShouldDisconnect_; }

    // SAX Handler iface
    void Null();
    void Bool(bool b);
    void Int(int i);
    void Uint(unsigned i);
    void Int64(int64_t i);
    void Uint64(uint64_t i);
    void Double(double d);
    void String(const Ch* str, rapidjson::SizeType length, bool copy);
    void StartObject();
    void EndObject(rapidjson::SizeType memberCount);
    void StartArray();
    void EndArray(rapidjson::SizeType elementCount);

private:
    enum TAG
    {
        TAG_NULL        = 0,
        TAG_FUNC        = 0x0001,
        TAG_OBJECT      = 0x0002,
        TAG_ON          = 0x0004,
        TAG_VAL         = 0x0008,
        TAG_STRVAL      = 0x0010,
        TAG_X           = 0x0020,
        TAG_Y           = 0x0040
    };

    enum FUNC
    {
        FUNC_INVALID = 0,
        FUNC_RESET_STATISTICS, FUNC_GET_ERROR_COUNTERS,
        FUNC_GET_VERSIONS,
        FUNC_WRITE_INPUT_MODE, FUNC_WRITE_INPUT_THRESOLD, FUNC_READ_INPUT, FUNC_WRITE_INPUT_PULLUP, FUNC_WRITE_INPUT_SHUNT,
        FUNC_WRITE_RELAY, FUNC_WATCHDOG,
        FUNC_WRITE_DAC,
        FUNC_DISPLAY_ENABLE, FUNC_DISPLAY_CLEAR, FUNC_DISPLAY_INVERT, FUNC_DISPLAY_SET_POS, FUNC_DISPLAY_TEXT, FUNC_DISPLAY_SELECT_FONT, FUNC_DISPLAY_SHOW,
        FUNC_READ_BUTTONS,
        FUNC_WRITE_LED,
        FUNC_WRITE_SPLASH,
        FUNC_DISCONNECT,
        FUNC_SHUTDOWN,
        FUNC_LAST_FUNC_DUMMY
    };

    enum OBJECT
    {
        OBJECT_INVALID = 0,
        OBJECT_INPUT_1, OBJECT_INPUT_2, OBJECT_INPUT_3, OBJECT_INPUT_4,
        OBJECT_RELAY_1, OBJECT_RELAY_2,
        OBJECT_DAC_1, OBJECT_DAC_2,
        OBJECT_LED_RS232_A, OBJECT_LED_RS232_B, OBJECT_LED_CAN, OBJECT_LED_READY, OBJECT_LED_STATUS, OBJECT_LED_ALARM, OBJECT_LED_UPLINK
    };

    void funcResetStats(WriteStream &writeStream);
    void funcGetErrorCounters(WriteStream &writeStream);
    void funcGetVersions(WriteStream &writeStream);
    void funcWriteInputMode(WriteStream &writeStream);
    void funcWriteInputThreshold(WriteStream &writeStream);
    void funcReadInput(WriteStream &writeStream);
    void funcWriteInputPullup(WriteStream &writeStream);
    void funcWriteInputShunt(WriteStream &writeStream);
    void funcWriteRelay(WriteStream &writeStream);
    void funcWatchdog(WriteStream &writeStream);
    void funcWriteDac(WriteStream &writeStream);
    void funcDisplayEnable(WriteStream &writeStream);
    void funcDisplayClear(WriteStream &writeStream);
    void funcDisplayInvert(WriteStream &writeStream);
    void funcDisplaySetPos(WriteStream &writeStream);
    void funcDisplayText(WriteStream &writeStream);
    void funcDisplaySelectFont(WriteStream &writeStream);
    void funcDisplayShow(WriteStream &writeStream);
    void funcReadButtons(WriteStream &writeStream);
    void funcWriteLed(WriteStream &writeStream);
    void funcWriteSplash(WriteStream &writeStream);
    void funcDisconnect(WriteStream &writeStream);
    void funcShutdown(WriteStream &writeStream);

    void        endCurrentTag();
    TAG         tagFromStr(const char *, size_t length);
    FUNC        funcFromStr(const char *, size_t length);
    OBJECT      objectFromStr(const char *, size_t length);
    void        writeStandardOk(WriteStream &writeStream);

    void checkObjectIs(OBJECT minObj, OBJECT maxObj);
    INPUT objectAsInput();
    RELAY objectAsRelay();
    DAC objectAsDac();
    LED objectAsLed();

    typedef void (JsonState::*HandlerFuncPtr)(WriteStream &);
    HandlerFuncPtr                  handlerFunc_;

    TAG                             currentTag_;
    uint32_t                        seenTags_;
    FUNC                            func_;
    OBJECT                          object_;
    bool                            on_;
    int                             intVal1_;
    int                             intVal2_;
    std::string                     strVal_;

    RESULT                          resultCode_;
    bool                            serverShouldShutdown_;
    bool                            serverShouldDisconnect_;
};


}
