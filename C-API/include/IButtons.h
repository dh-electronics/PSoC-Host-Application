#pragma once


#include "buttons.h"


enum RESULT;


namespace drc01
{


class IButtons
{
public:
    virtual BUTTONS read(RESULT *result)    =0;
    virtual BUTTONS get() const             =0;
};


} // namespace bigfish
