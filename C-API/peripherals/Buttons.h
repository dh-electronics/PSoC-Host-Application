#pragma once


#include "IButtons.h"
#include <Poco/Mutex.h>


enum RESULT;


namespace bigfish
{


class SpiProto;


class Buttons : public IButtons
{
public:
    static const BUTTONS ZERO_BUTTONS;

    Buttons(SpiProto &proto);
    virtual ~Buttons() {}

    virtual BUTTONS read(RESULT *result);
    virtual BUTTONS get() const { return buttons_; }

private:
    SpiProto &              proto_;
    BUTTONS                 buttons_;
    mutable Poco::FastMutex accessMutex_;
};


} // namespace bigfish
