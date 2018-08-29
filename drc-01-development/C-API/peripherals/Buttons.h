#pragma once


#include "IButtons.h"
#include <Poco/Mutex.h>


enum RESULT;


namespace drc01
{


class SpiProto;


class Buttons : public IButtons
{
public:
    Buttons(SpiProto &proto);
    virtual ~Buttons() {}

    virtual void   setCallback(buttonsCallback *callback);
    virtual RESULT handleEvents();
    virtual bool   getState(enum BUTTON button) const;

private:
    static const int NUM_BUTTONS = 5;
    SpiProto &      proto_;
    buttonsCallback *callback_;
    bool            states_[NUM_BUTTONS];

    mutable Poco::FastMutex accessMutex_;
};


}
