// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2014-2023 DH electronics GmbH

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

