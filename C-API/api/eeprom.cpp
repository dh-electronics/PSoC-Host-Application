#include <c-api/eeprom.h>
#include <IApiManager.h>
#include <peripherals/IEeprom.h>
#include <cassert>


using namespace drc01;


enum RESULT writeEeprom(unsigned short *data, unsigned short offset, unsigned char wordCount)
{
    IApiManager *man = IApiManager::getIface();
    if(man)
    {
        man->active();
        IEeprom *eeprom = man->getEeprom();
        assert(eeprom);
        return eeprom->write(data, offset, wordCount);
    }
    else
    {
        return RESULT_API_NOT_OPEN;
    }
}


