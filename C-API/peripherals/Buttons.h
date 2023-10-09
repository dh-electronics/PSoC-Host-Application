// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#pragma once


#include "peripherals/IButtons.h"
#include <mutex>


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
    static const int        NUM_BUTTONS = 5;
    SpiProto &              proto_;
    buttonsCallback         *callback_;
    bool                    states_[NUM_BUTTONS];

    mutable std::mutex      accessMutex_;
};


}
