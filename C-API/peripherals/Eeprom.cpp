#include "peripherals/Eeprom.h"
#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/SpiProto.h"
#include "spi/MasterHelpers.h"
#include <string.h>


using namespace drc01;


Eeprom::Eeprom(SpiProto &proto)
    : proto_(proto)
{

}


RESULT Eeprom::write(uint16_t *data, uint16_t byteOffset, uint8_t wordCount)
{
    const uint8_t chunkSizeWords = (SPI_PACKET_SIZE - 3) >> 1; // CMD, 1byte OFFSET, CRC,
    const uint8_t wordOffset = (uint8_t)(byteOffset >> 1);

    for(uint8_t word = 0; word < wordCount; )
    {
        const uint8_t wordsLeft = wordCount - word;
        const uint8_t wordsToXfer = (wordsLeft > chunkSizeWords) ? chunkSizeWords : wordsLeft;
        const uint8_t bytesToXfer = (uint16_t) wordsToXfer << 1;

        CommandUnion cu;
        cu.cmd.command_ = CMD_EEPROM_WRITE;
        set_8(cu.cmd.data_, 0, wordOffset + word);
        uint8_t cmdLength = bytesToXfer + 3;
        memcpy(&cu.cmd.data_[1], &data[word], bytesToXfer);
        cmdCalcCrc(&cu.cmd, cmdLength);

        ResponseUnion ru;
        const RESULT res = proto_.xmit(&cu.cmd, cmdLength, &ru.rsp, 2, EEPROM_WAITTIME);
        if(RESULT_OK != res)
            return res;
        word += wordsToXfer;
    }

    return RESULT_OK;
}



