// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#pragma once


#include "peripherals/ILed.h"
#include <mutex>


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
    mutable std::mutex      accessMutex_;
};


}
