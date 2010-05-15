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
#include <Jet/Object.hpp>
#include <vector>
#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#endif

#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace Jet { namespace Core {

//! Reads and writes data to a multicast, unicast TCP, or unicast UDP socket.
//! @class Socket
//! @brief Reads and writes data to a multicast, unicast TCP, or unicast UDP
//! socket.  For TCP sockets, data is read as a packet even though the
//! underlying protocol is stream-oriented.
class Socket : public Object {
public:
    //! Destructor
    ~Socket();
    
	//! Creates a unicast TCP socket connecting to the given port and IP
    //! address.
    //! @param ip the destination address
    //! @param port the destination port
    static Socket* client(const std::string& ip, uint16_t port);
    
    //! Creates a unicast TCP socket serving on the given port and IP
    //! address.
    //! @param ip the destination address
    //! @param port the destination port
    static Socket* server(uint16_t port);
    
    //! Creates a multicast UDP socket on the given port and using the
    //! specified multicast IP.  Returns immediately if the port is free,
    //! or throws an std::runtime_error if the operation fails.
    //! @param ip the destination address
    //! @param port the destination port
    static Socket* multicast(const std::string& ip, uint16_t port);
    
    //! Creates a unicast UDP socket on the given port.  Returns immediately
    //! if the port is free, or throws an std::runtime_error if the operation
    //! fails
    //! @param ip the group ip
    //! @param port the group port
    static Socket* datagram(const std::string& ip, uint16_t port);
        
    //! Returns true if data is available for reading.
    bool incoming_packet() {
        boost::mutex::scoped_lock lock(in_mutex_);
        return in_packet_;
    }
    
    //! Returns true if the socket is connected.
    bool connected() {
        boost::mutex::scoped_lock lock(connected_mutex_);
        return connected_;
    }
   
private:
    Socket(const sockaddr_in& local, const sockaddr_in& remote, SocketType type);
    
    void init();
    void init_multicast();
    void init_client();
    void init_server();
    void init_datagram();
    
    void run_in_thread();
    void run_out_thread();
    void do_stream_receive();
    void do_stream_send();
    void do_packet_receive();
    void do_packet_send();
    
    int socket_;
    sockaddr_in local_;
    sockaddr_in remote_;
    std::vector<char> in_;
    std::vector<char> out_;
    boost::thread init_thread_;
    boost::thread in_thread_;
    boost::thread out_thread_;
    boost::mutex in_mutex_;
    boost::mutex out_mutex_;
    boost::mutex connected_mutex_;
    boost::condition_variable in_condition_;
    boost::condition_variable out_condition_;
    boost::condition_variable ready_condition_;
    bool out_packet_;
    bool in_packet_;
    bool quit_;
    bool connected_;
    SocketType type_;
    
    friend class SocketReader;
    friend class SocketWriter;
};

}}
