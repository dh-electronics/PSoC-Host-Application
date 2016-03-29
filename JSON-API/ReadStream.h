#pragma once


#include <Poco/Net/StreamSocket.h>
#include <stdint.h>
#include <cassert>


namespace bigfish
{


class ReadStream
{
public:
    static const uint32_t TIMEOUT = 60000; // millisecs in 1 minute

    typedef char Ch;	//!< Character type of the stream.

    explicit ReadStream(Poco::Net::StreamSocket &socket);
    ReadStream(const ReadStream &other);

    ReadStream & operator = (const ReadStream &other);

    //! Read the current character from stream without moving the read cursor.
    Ch Peek() const;

    //! Read the current character from stream and moving the read cursor to next character.
    Ch Take();

    //! Get the current read cursor.
    //! \return Number of characters read from start.
    size_t Tell();

    // unneeded funcs - unimplemented
    Ch* PutBegin()              { assert(false); return 0; }
    void Put(Ch)                { assert(false); }
    void Flush()                { assert(false); }
    size_t PutEnd(Ch*)          { assert(false); return 0; }

private:
    char read() const;

    Poco::Net::StreamSocket *   socket_;
    mutable int16_t             current_;
    mutable size_t              count_;
};


}
