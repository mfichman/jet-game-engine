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

#include <Jet/Network/BSockTypes.hpp>
#include <Jet/Object.hpp>
#include <vector>

namespace Jet { 

//! Reads and writes data to a multicast, unicast TCP, or unicast UDP socket.
//! @class BSockSocket
//! @brief Reads and writes data to a multicast, unicast TCP, or unicast UDP
//! socket.  For TCP sockets, data is read as a packet even though the
//! underlying protocol is stream-oriented.
class BSockServerSocket : public Object {
public:
    //! Destructor
    ~BSockServerSocket();
    
	//! Creates a unicast TCP socket connecting to the given port and IP
    //! address.
    //! @param ip the destination address
    //! @param port the destination port
    static BSockServerSocket* server(uint16_t port=0);
        
    //! Returns a socket if one is currently connected
    BSockSocket* socket();
    
    //! Returns the port
    inline uint16_t port() const {
        return port_;
    }
    //! Returns the ip address
    inline const std::string& address() const {
        return address_;
    }
   
private:
    BSockServerSocket(const sockaddr_in& local, const sockaddr_in& remote);
    
    int socket_;
    uint16_t port_;
    std::string address_;
    sockaddr_in local_;
    sockaddr_in remote_;
};

}
