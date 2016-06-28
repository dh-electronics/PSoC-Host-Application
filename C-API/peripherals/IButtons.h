#pragma once


#include "buttons.h"


enum RESULT;


namespace drc01
{


class IButtons
{
public:
    virtual RESULT setCallback(buttonsCallback *callback)   =0;
    virtual RESULT handleButtons()                                 =0;
};


}
