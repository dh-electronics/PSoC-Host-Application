// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2016-2023 DH electronics GmbH

#include "peripherals/Led.h"
#include <iostream>


using namespace drc01;
using namespace std;


Led::Led()
    : state_(false)
{
}


void Led::restore()
{
    // cout << "Led: restoring value" << endl;
    write(state_);
}
