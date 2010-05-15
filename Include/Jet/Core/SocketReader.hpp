/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  
#pragma once

#include <Jet/Core/Types.hpp>
#include <Jet/Core/Socket.hpp>
#include <Jet/Object.hpp>
#include <boost/thread.hpp>

namespace Jet { namespace Core {

//! Reads data from a socket synchronously.
//! @class SocketReader
//! @brief Reads data from a socket synchronously.
class SocketReader : public Object {
public:
    //! Creates a new socket reader and locks the buffer for reading.
    SocketReader(Socket* socket);
    
    //! Destructor
    ~SocketReader();

    //! Reads floating-point data from the socket.  Throws an exception if data
    //! is not available.  Does not block.
    float real();
    
    //! Reads integer data from the socket.  Throws an exception if data is not
    //! available.  Does not block.
    int integer();
    
    //! Reads a string from the socket.  Throws an exception if data is not
    //! available.  Does not block.
    std::string string();
    
private:
    SocketPtr socket_;
    boost::mutex::scoped_lock lock_;
    size_t bytes_read_;
};

}}
