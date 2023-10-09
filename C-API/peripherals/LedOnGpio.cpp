// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#include "peripherals/LedOnGpio.h"
#include "spi/SpiProto.h"


using namespace drc01;


LedOnGpio::LedOnGpio(SpiProto &proto, GPIO gpio)
    : proto_(proto)
    , gpio_(gpio)
{
}


RESULT LedOnGpio::write(bool on)
{
    std::lock_guard<std::mutex> lock(accessMutex_);
    const RESULT res = proto_.gpioControl(gpio_, on);
    if(RESULT_OK == res)
        state_ = on;
    return res;
}
