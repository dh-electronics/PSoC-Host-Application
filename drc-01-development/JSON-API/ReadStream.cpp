#include "ReadStream.h"
#include <Poco/Net/NetException.h>
#include <stdio.h>
#include <string.h>
#include <iostream>


using namespace drc01;
using namespace Poco;
using namespace Poco::Net;
using namespace std;


ReadStream::ReadStream(StreamSocket &socket)
    : socket_(&socket)
    , current_(-1)
    , count_(0)
{
}


ReadStream::ReadStream(const ReadStream &other)
{
    *this = other;
}


ReadStream & ReadStream::operator = (const ReadStream &other)
{
    socket_ = other.socket_;
    current_ = other.current_;
    count_ = other.count_;
    return *this;
}


ReadStream::Ch ReadStream::Peek() const
{
    if(current_ < 0)
        current_ = read();

    // cout << "Peek: " << current_ << flush;
    return current_;
}


ReadStream::Ch ReadStream::Take()
{
    Ch c;
    if(current_ < 0)
    {
        c = read();
    }
    else
    {
        c = current_;
        current_ = -1;
    }

    // cout << "Take: " << c << flush;
    return c;
}


size_t ReadStream::Tell()
{
    return count_;
}


char ReadStream::read() const
{
    if(socket_->poll(TIMEOUT * 1000, Socket::SELECT_READ))
    {
        char c;
        int n = socket_->receiveBytes(&c, 1, MSG_NOSIGNAL); // this may throw a NetException - catched in JsonServer
        if(n == 1)
        {
            ++count_;
            // cout << "ReadStream::read: " << c << endl;
            return c;
        }
    }

    // we are at eof or timeout
    return '\0'; // end of data
}
