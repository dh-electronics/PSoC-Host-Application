// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#pragma once


#include <c-api/resultcodes.h>


namespace drc01
{


class ILed
{
public:
    virtual ~ILed() {}
    virtual RESULT write(bool on)   =0;
    virtual int get() const         =0;
};


}
