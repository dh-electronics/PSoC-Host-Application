/* 
 * File:   MasterHelpers.h
 * Author: pkishalov
 *
 * Created on 5. August 2014, 15:41
 */

#ifndef MASTERHELPERS_H
#define	MASTERHELPERS_H


#include <spi/Command.h>
#include <spi/Response.h>
#include <spi/GenericHelpers.h>


namespace drc01 {


void        cmdCalcCrc(GenericCommand *cmd, uint8_t length);
enum ERR    rspErrorCode(GenericResponse *rsp);
int         rspIsBusy(GenericResponse *rsp);
int         rspCheckCrc(GenericResponse *rsp);


template <uint8_t dataLength>
class Response
{
public:
    explicit Response() { rspu_.rsp.length_ = getSize(); }

    uint8_t getSize() const { return dataLength + sizeof(rspu_.rsp) + 1; }
    uint8_t         *data()             { return rspu_.rsp.data_; }
    const uint8_t   *data() const       { return rspu_.rsp.data_; }
    GenericResponse         *rsp()          { return &rspu_.rsp; }
    const GenericResponse   *rsp() const    { return &rspu_.rsp; }

    bool isError() const    { return rspIsError(&rspu_.rsp); }
    ERR errorCode() const   { return rspErrorCode(&rspu_.rsp); }
    bool isBusy() const     { return rspIsBusy(&rspu_.rsp); }
    bool checkCrc() const   { return rspCheckCrc(&rspu_.rsp); }

private:
    ResponseUnion rspu_;
};


template <uint8_t dataLength>
struct Command
{
public:
    explicit Command(CMD command) { cmdu_.cmd.command_ = command; }

    uint8_t getSize() const { return dataLength + sizeof(cmdu_.cmd) + 1; }
    uint8_t         *data()         { return cmdu_.cmd.data_; }
    const uint8_t   *data() const   { return cmdu_.cmd.data_; }
    GenericCommand          *cmd()          { return &cmdu_.cmd; }
    const GenericCommand    *cmd() const    { return &cmdu_.cmd; }

    void calcCrc() { cmdCalcCrc(&cmdu_.cmd, getSize()); }

private:
    CommandUnion cmdu_;
};


}


#endif	/* MASTERHELPERS_H */

