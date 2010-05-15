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

#include <Jet/Core/Socket.hpp>
#include <stdexcept>
#include <cstring>
#include <iostream>

using namespace Jet;
using namespace std;

#ifdef WINDOWS
const char* errmsg() {
    static char buffer[512];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), NULL, buffer, 512, NULL);
    return buffer;
}    
int err() {
    return WSAGetLastError();
}
#else
#define INVALID_SOCKET -1
#define errmsg() strerror(errno)
#define err errno
#define closesocket close
#endif

Core::Socket* Core::Socket::server(uint16_t port) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = htons(port); // Choose any port for TCP

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = 0;
    
    return new Core::Socket(local, remote, SERVER);
}

Core::Socket* Core::Socket::client(const std::string& ip, uint16_t port) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = 0; // Choose any port for TCP

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(ip.c_str());
    remote.sin_port = htons(port);
    
    return new Core::Socket(local, remote, CLIENT);
}

Core::Socket* Core::Socket::multicast(const std::string& ip, uint16_t port) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = htons(port); // Bind local to any given port

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(ip.c_str());
    remote.sin_port = htons(port);
    
    return new Core::Socket(local, remote, MULTICAST);

}

Core::Socket* Core::Socket::datagram(const std::string& ip, uint16_t port) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = 0; // Bind local to any given port

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(ip.c_str());
    remote.sin_port = htons(port);
    
    return new Core::Socket(local, remote, DATAGRAM);
}

Core::Socket::Socket(const sockaddr_in& local, const sockaddr_in& remote, SocketType type) :
    socket_(INVALID_SOCKET),
    local_(local),
    remote_(remote),
    in_(4096),
    out_(4096),
    out_packet_(false),
    in_packet_(false),
    quit_(false),
    connected_(false),
	type_(type) {
        
    init_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::init), this));
}

Core::Socket::~Socket() {
    quit_ = true;
    
	out_mutex_.lock();
	out_packet_ = true;
	out_condition_.notify_all();
	out_mutex_.unlock();
        
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

	if (socket_ != INVALID_SOCKET) {
		shutdown(socket_, SD_BOTH);
	    
		//! Set socket option for receive timeouts
		if (setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv))) {
			throw runtime_error(errmsg());
		}
	    
		//! Set socket option for receive timeouts
		if (setsockopt(socket_, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv))) {
			throw runtime_error(errmsg());
		}
	}
    
    in_thread_.join();
    out_thread_.join();
	
    if (socket_ != INVALID_SOCKET) {
        closesocket(socket_);
    }
}

void Core::Socket::init() {
	try {
		switch (type_) {
			case DATAGRAM: init_datagram(); break;
			case MULTICAST: init_multicast(); break;
			case SERVER: init_server(); break;
			case CLIENT: init_client(); break;
			default: return;
		}
	} catch (std::runtime_error& ex) {
		cout << ex.what() << endl;
	}
}

void Core::Socket::init_server() {
    // Create a new TCP socket.  If the attempt fails, throw an exception.
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(errmsg());
    }
    
    // Attempt to bind the socket to any port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(errmsg());
    }

	// Listen for connections
	if (listen(socket_, 1) < 0) {
		throw runtime_error(errmsg());
	}
    
    // Attempt to accept a socket
    int socklen = sizeof(remote_);
    int sd = accept(socket_, (sockaddr*)&remote_, &socklen);
    if (quit_) {
        return;
    } else if (INVALID_SOCKET == sd) {
        throw runtime_error(errmsg());
    }
    
    // Shut down the server socket, and start using the connection socket.
    shutdown(socket_, SD_BOTH);
    closesocket(socket_);
    socket_ = sd;
    
    in_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_in_thread), this));
    out_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_out_thread), this));
    boost::mutex::scoped_lock lock(connected_mutex_);
    connected_ = true;
}

void Core::Socket::init_client() {    
    // Create a new TCP socket.  If the attempt fails, throw an exception.
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(errmsg());
    }
    
    // Attempt to bind the socket to any port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(errmsg());
    }
    
    // Attempt to bind the socket to the given port.  If the attempt fails,
    // clean close the socket and throw an exception.
    int rt = connect(socket_, (sockaddr*)&remote_, sizeof(remote_));
    if (quit_) {
        return;
    } else if (rt < 0) {
        throw runtime_error(errmsg());
    }
    
    in_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_in_thread), this));
    out_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_out_thread), this));
    boost::mutex::scoped_lock lock(connected_mutex_);
    connected_ = true;
}

void Core::Socket::init_multicast() {    
    ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = remote_.sin_addr.s_addr;
    mreq.imr_interface.s_addr = local_.sin_addr.s_addr;
    
    // Create a new UDP socket.  If the attempt fails, throw an exception.
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(errmsg());
    }
    
    // Set the port to be reusable.  If the attempt fails, close the socket
    // and throw an exception.
    int yes = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) < 0) {
        throw runtime_error(errmsg());
    }
    
    // Join the multicast group.  If the attempt fails, close the socket and
    // throw and exception.
    if (setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(ip_mreq)) < 0) {
        throw runtime_error(errmsg());
    }
    
    // Attempt to bind the socket to the given port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(errmsg());
    }

    in_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_in_thread), this));
    out_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_out_thread), this));
    boost::mutex::scoped_lock lock(connected_mutex_);
    connected_ = true;
}

void Core::Socket::init_datagram() {
    
    // Create a new UP socket.  If the attempt fails, throw an exception.
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(errmsg());
    }
    
    // Set the port to be reusable.  If the attempt fails, close the socket
    // and throw an exception.
    int yes = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) < 0) {
        throw runtime_error(errmsg());
    }
    
    // Attempt to bind the socket to the given port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(errmsg());
    }
    
    in_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_in_thread), this));
    out_thread_ = boost::thread(boost::bind(boost::mem_fn(&Socket::run_out_thread), this));
    boost::mutex::scoped_lock lock(connected_mutex_);
    connected_ = true;
}

void Core::Socket::do_packet_send() {
    // Wait until a packet has been queued to be sent using this socket.
    boost::mutex::scoped_lock lock(out_mutex_);
    while (!out_packet_) {
        out_condition_.wait(lock);
    }
	if (out_.empty()) {
		out_packet_ = false;
		out_condition_.notify_all();
		return;
	}
    int socklen = sizeof(remote_);
    
    // Write the length of the packet into the header.  This includes the
    // length of the header itself.
    *(size_t*)&out_[0] = htonl(out_.size());
    
    // Get the packet pointer and length
    char* pkt = &out_[0];
    size_t len = out_.size();
        
    // Write to the socket
    int rt = sendto(socket_, pkt, len, 0, (sockaddr*)&remote_, socklen);
        
    // If an error occurred, or the socket is already closed, then throw
    // an exception
	if (quit_) {
		return;
	} else if (rt < 0) {
        throw runtime_error(errmsg());
    } else if (rt != len) {
        throw runtime_error("Failed to send datagram");
    }

	out_packet_ = false;
    out_condition_.notify_all();
}

void Core::Socket::do_packet_receive() {
    // Wait until the current packet has been read by a reader before reading
    // the next packet.
    boost::mutex::scoped_lock lock(in_mutex_);
    while (in_packet_) {
        in_condition_.wait(lock);
    }
    lock.unlock();
    sockaddr_in source;
    int socklen = sizeof(source);

    // Assume the packet will be 4096 bytes, max.
    in_.resize(4096);
    
    // Get the packet pointer and length
    char* pkt = &in_[0];
    size_t len = in_.size();
        
    // Read from the socket, and capture the source addres.
    int rt = recvfrom(socket_, pkt, len, 0, (sockaddr*)&source, &socklen);

        
    // If an error occurred, or the socket is already closed, then throw an
    // exception.
	if (quit_) {
		return;
	} else if (rt < 0) {
        throw runtime_error(errmsg());
    } else if (rt < sizeof(size_t)) {
        throw runtime_error("Invalid packet");
    }
    
    // Read the length of the packet from the beginning of the packet, as
    // long as there are at least 4 bytes in the packet (size of an int).
    in_.resize(ntohl(*(size_t*)&in_[0]));

	in_packet_ = true;
    in_condition_.notify_all();
}

void Core::Socket::do_stream_send() {
    // Wait until a packet has been queued to be sent using this socket.
    boost::mutex::scoped_lock lock(out_mutex_);
    while (!out_packet_) {
        out_condition_.wait(lock);
    }
	if (out_.empty()) {
		out_packet_ = false;
		out_condition_.notify_all();
		return;
	}
    
    // This is the number of bytes that have been sent from the socket
    size_t sent_bytes = 0;
    
    // Write the length of the packet into the header.  This includes the
    // length of the header itself.
    *(size_t*)&out_[0] = htonl(out_.size());
    
    // If the number of sent bytes is greater than or equal to the number of
    // bytes in the output buffer, then we have reached the end of the buffer
    // and the whole packet has been sent.
    while (sent_bytes < out_.size()) {
        char* pkt = &out_[sent_bytes];
        size_t len = out_.size() - sent_bytes;
        
        // Write to the socket
        int rt = send(socket_, pkt, len, 0);
        
        // If an error occurred, or the socket is already closed, then throw
        // an exception
		if (quit_) {
			return;
		} else if (rt < 0) {
            throw runtime_error(errmsg());
        } else {
            sent_bytes += rt;
        }        
    }
    
    out_packet_ = false;
    out_condition_.notify_all();
}

void Core::Socket::do_stream_receive() {
    // Wait until the current packet has been read by a reader before reading
    // the next packet.
    boost::mutex::scoped_lock lock(in_mutex_);
    while (in_packet_) {
        in_condition_.wait(lock);
    }
    lock.unlock();

    // Number of bytes read, used to track progress in receiving the packet
    size_t read_bytes = 0;
    
    // Assume the packet will be 4 bytes.  This is the size of a minimum
    // packet, which ontains only the length header.  This value will be
    // adjusted once the actual length of the packet is read from the header.
    in_.resize(sizeof(size_t));
    
    // If the length of the buffer is greater than the header says, then
    // we have reached the end of the current packet. 
    while (read_bytes < in_.size()) {
        char* pkt = &in_[read_bytes];
        size_t len = in_.size() - read_bytes;   
        
        // Read from the socket, and capture the source addres.
        int rt = recv(socket_, pkt, len, 0);
        
        // If an error occurred, or the socket is already closed, then throw an
        // exception.
		if (quit_) {
			return;
		} else if (rt < 0) {
            throw runtime_error(errmsg());
        } else if (rt == 0) {
            throw runtime_error("Socket is closed");
        } else {
            read_bytes += rt;
        }
        
        // Read the length of the packet from the beginning of the packet, as
        // long as there are at least 4 bytes in the packet (size of an int).
        if (read_bytes >= sizeof(size_t)) {
            in_.resize(ntohl(*(size_t*)&in_[0]));
        }
    }
    
    in_packet_ = true;
    in_condition_.notify_all();
}

void Core::Socket::run_in_thread() {
    try {
		if (DATAGRAM == type_ || MULTICAST == type_) {
			while (!quit_) {
				do_packet_receive();
			}
		} else if (CLIENT == type_ || SERVER == type_) {
			while (!quit_) {
				do_stream_receive();
			}
		}
    } catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    
    boost::mutex::scoped_lock lock(connected_mutex_);
    connected_ = false;
}

void Core::Socket::run_out_thread() {
    try {
		if (DATAGRAM == type_ || MULTICAST == type_) {
			while (!quit_) {
				do_packet_send();
			}
		} else if (CLIENT == type_ || SERVER == type_) {
			while (!quit_) {
				do_stream_send();
			}
		}
    } catch(std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    
    boost::mutex::scoped_lock lock(connected_mutex_);
    connected_ = false;
}