#pragma once


#include <ILed.h>
#include "resultcodes.h"
#include <Poco/Mutex.h>


namespace bigfish
{


class Led : public ILed
{
public:
    static const int COUNT     = 7;

    virtual int get() const { return state_; }
    void restore();

protected:
    Led();

    bool                    state_;
    mutable Poco::FastMutex accessMutex_;
};


} // namespace bigfish
