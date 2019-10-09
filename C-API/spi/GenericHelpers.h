#ifndef GENERICHELPERS_H
#define GENERICHELPERS_H


#include "spi/Response.h"


#ifdef __cplusplus
extern "C" {
#endif


void calcCrc(uint8_t *data, uint8_t length);
int checkCrc(const uint8_t *data, uint8_t length);
int rspIsError(struct GenericResponse *rsp);


uint8_t         get_8(const uint8_t *data, uint8_t offset);
uint16_t        get_16(const uint8_t *data, uint8_t offset);
uint32_t        get_32(const uint8_t *data, uint8_t offset);
void            set_8(uint8_t *data, uint8_t offset, uint8_t value);
void            set_16(uint8_t *data, uint8_t offset, uint16_t value);
void            set_32(uint8_t *data, uint8_t offset, uint32_t value);


#ifdef	__cplusplus
}
#endif


#endif // GENERICHELPERS_H
