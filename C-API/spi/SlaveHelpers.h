/* 
 * File:   SlaveHelpers.h
 * Author: pkishalov
 *
 * Created on 5. August 2014, 15:47
 */

#ifndef SLAVEHELPERS_H
#define	SLAVEHELPERS_H


#include "spi/Command.h"
#include "spi/Response.h"
#include "spi/GenericHelpers.h"


#ifdef	__cplusplus
extern "C" {
#endif


int cmdCheckCrc(struct GenericCommand *cmd, uint8_t length);
void rspCalcCrc(struct GenericResponse *rsp);


#ifdef	__cplusplus
}
#endif

#endif	/* SLAVEHELPERS_H */

