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
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Types/Address.hpp>
#include <Jet/Object.hpp>
#include <vector>
#include <queue>

namespace Jet {

//! Reads and writes data to a multicast, unicast TCP, or unicast UDP socket.
//! @class BSockSocket
//! @brief Reads and writes data to a multicast, unicast TCP, or unicast UDP
//! socket.  For TCP sockets, data is read as a packet even though the
//! underlying protocol is stream-oriented.
class BSockSocket : public Object {
public:
    //! Destructor
    ~BSockSocket();
    
	//! Creates a unicast TCP socket connecting to the given port and IP
    //! address.
    //! @param ip the destination address
    //! @param port the destination port
    static BSockSocket* client(CoreEngine* engine, const Address& addres);
    
    //! Creates a unicast TCP socket serving on the given port and IP
    //! address.
    //! @param ip the destination address
    //! @param port the destination port
    static BSockSocket* server(CoreEngine* engine, const Address& address);
    
    //! Creates a multicast UDP socket on the given port and using the
    //! specified multicast IP.  Returns immediately if the port is free,
    //! or throws an std::runtime_error if the operation fails.
    //! @param ip the destination address
    //! @param port the destination port
    static BSockSocket* multicast(CoreEngine* engine, const Address& address);
    
    //! Creates a unicast UDP socket on the given port.  Returns immediately
    //! if the port is free, or throws an std::runtime_error if the operation
    //! fails
    //! @param ip the group ip
    //! @param port the group port
    static BSockSocket* datagram(CoreEngine* engine, const Address& address);
        
    //! Returns a packet writer for this socket, or null if no data is
    //! available.
    BSockWriter* writer();
    
    //! Returns a socket reader for this socket, or null if no data is
    //! available.
    BSockReader* reader();
    
    //! Receives data if possible
    void poll_read();
    
    //! Send data if possible
    void poll_write();
    
    //! Returns the port
    inline const Address& address() const {
        return address_;
	}
   
private:
    BSockSocket(CoreEngine* engine, const sockaddr_in& local, const sockaddr_in& remote, SocketType type, int socket=INVALID_SOCKET);
    
    void init_multicast();
    void init_client();
    void init_server();
    void init_datagram();
    
    void accept();
    void connect();
    void read_stream();
    void read_datagram();
    void write_stream();
    void write_datagram();
    
	CoreEngine* engine_;
    int socket_;
    sockaddr_in local_;
    sockaddr_in remote_;
    std::vector<char> in_;
    std::queue<std::vector<char> > out_;
    SocketType type_;
    size_t write_bytes_;
    size_t read_bytes_;
	Address address_;
    
    friend class BSockReader;
    friend class BSockWriter;
    friend class BSockServerSocket;
};

}
