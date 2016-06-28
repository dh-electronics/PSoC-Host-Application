#pragma once


#include <peripherals/ILed.h>
#include "resultcodes.h"
#include <Poco/Mutex.h>


namespace drc01
{


class Led : public ILed
{
public:
    static const int COUNT     = 4;

    virtual int get() const { return state_; }
    void restore();

protected:
    Led();

    bool                    state_;
    mutable Poco::FastMutex accessMutex_;
};


}
