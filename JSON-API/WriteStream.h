#pragma once


#include <Poco/Net/StreamSocket.h>
#include <cassert>


namespace drc01
{


class WriteStream
{
public:
    static const uint32_t TIMEOUT = 500; // millisecs
    static const size_t BUFFER_SIZE = 64;

    typedef char Ch;    //!< Character type of the stream.
    explicit WriteStream(Poco::Net::StreamSocket &socket);

    //! Write a character.
    void Put(Ch c);

    //! Flush the buffer.
    void Flush();

    // unneeded funcs - unimplemented
    Ch Peek() const             { assert(false); return 0; }
    Ch Take()                   { assert(false); return 0; }
    size_t Tell()               { assert(false); return 0; }
    Ch* PutBegin()              { assert(false); return 0; }
    size_t PutEnd(Ch*)          { assert(false); return 0; }

private:
    Poco::Net::StreamSocket &   socket_;
    Ch *                        current_;
    Ch * const                  end_;
    Ch                          buffer_[BUFFER_SIZE];
};


}
