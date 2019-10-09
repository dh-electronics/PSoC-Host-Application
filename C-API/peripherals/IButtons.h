#pragma once


#include <c-api/buttons.h>


enum RESULT;


namespace drc01
{


class IButtons
{
public:
    virtual void   setCallback(buttonsCallback *callback)   =0;
    virtual RESULT handleEvents()                           =0;
    virtual bool   getState(enum BUTTON button) const       =0;

};


}
