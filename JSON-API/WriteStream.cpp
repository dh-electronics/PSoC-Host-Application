#include "WriteStream.h"
#include <Poco/Net/NetException.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>


using namespace drc01;
using namespace Poco;
using namespace Poco::Net;


WriteStream::WriteStream(StreamSocket &socket)
    : socket_(socket)
    , current_(buffer_)
    , end_(buffer_ + BUFFER_SIZE)
{
}


void WriteStream::Put(WriteStream::Ch c)
{
    if(current_ >= end_)
        Flush();

    *current_++ = c;
}


void WriteStream::Flush()
{
    if(current_ != buffer_)
    {
        // int sent = socket_.sendBytes(buffer_, bytesSent, 0);
        // here is a workaround around a bug in Poco::Socket.send(),
        // which blocks forever on a broken connection
        int sock = socket_.impl()->sockfd();
        size_t bytesSent = (size_t)(current_ - buffer_);
        if(send(sock, buffer_, bytesSent, MSG_NOSIGNAL) == -1)
            throw NetException(errno);
        current_ = buffer_;
    }
}
