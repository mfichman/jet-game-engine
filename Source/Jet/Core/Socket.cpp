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
#include <Jet/Core/SocketWriter.hpp>
#include <Jet/core/SocketReader.hpp>
#include <stdexcept>
#include <cstring>
#include <iostream>

using namespace Jet;
using namespace std;

#ifdef WINDOWS
#define EWOULDBLOCK WSAEWOULDBLOCK
typedef socklen_t int
const char* errmsg() {
    static char buffer[512];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), NULL, buffer, 512, NULL);
    return buffer;
}    
int errcode() {
    return WSAGetLastError();
}
#else
#define SD_BOTH SHUT_RDWR
#define INVALID_SOCKET -1
#define errmsg() strerror(errno)
#define errcode() errno
#define closesocket close
#include <unistd.h>
#include <fcntl.h>
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
	type_(type),
    write_bytes_(0),
    read_bytes_(0) {
        
    switch (type_) {
        case DATAGRAM: init_datagram(); break;
        case MULTICAST: init_multicast(); break;
        case SERVER: init_server(); break;
        case CLIENT: init_client(); break;
        default: return;
    }
    
#ifdef WINDOWS
    // Use non-blocking sockets
    u_long yes = 1;
    if (ioctlsocket(socket_, FIONBIO, &yes) < 0) {
        throw runtime_error(errmsg());
    }
#else
	int flags = fcntl(socket_, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(socket_, F_SETFL, flags);
#endif

}

Core::Socket::~Socket() {	
    if (socket_ != INVALID_SOCKET) {
#ifdef WINDOWS
		shutdown(socket_, SD_BOTH);
#endif
        closesocket(socket_);
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

	// Attempt to bind the socket to the given port.  This is non-blocking,
    // so the socket will return EWOULDBLOCK.  If an error would have happened
    // for a blocking connect, then we will catch that error on the first read
    // or write.
    if (::connect(socket_, (sockaddr*)&remote_, sizeof(remote_)) < 0) {
        if (EWOULDBLOCK != errcode()) {
            throw runtime_error(errmsg());
        }
    }
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
}

void Core::Socket::poll_read() {
    
    // Attempt to accept the incoming connection
    if (SERVER == type_) {
        accept();
    } else if (CLIENT == type_) {
        connect();
    }
    
    // If the input buffer is empty or a read is in progress, then continue
    // reading the packet if data is available.
    if (in_.empty() || in_.size() != read_bytes_) {
        if (MULTICAST == type_ || DATAGRAM == type_) {
            read_datagram();
        } else if (STREAM == type_) {
            read_stream();
        }
    }
}

void Core::Socket::poll_write() {
    // Attempt to accept the incoming connection
    if (SERVER == type_) {
        accept();
    } else if (CLIENT == type_) {
        connect();
    }
    
    // If the output buffer is not empty, then continue writing the packet
    // if writing is permissible.
    if (!out_.empty()) {
        if (MULTICAST == type_ || DATAGRAM == type_) {
            write_datagram();
        } else if (STREAM == type_) {
            write_stream();
        }
    } 
}

void Core::Socket::accept() {
    // Attempt to accept a socket

    socklen_t socklen = sizeof(remote_);
	int sd = ::accept(socket_, (sockaddr*)&remote_, &socklen);
	if (INVALID_SOCKET == sd) {
		if (EWOULDBLOCK == errcode()) {
			return;
		} else {
			throw runtime_error(errmsg());
		}
    }
    
    // Shut down the server socket, and start using the connection socket.
    shutdown(socket_, SD_BOTH);
    closesocket(socket_);
    socket_ = sd;
    type_ = STREAM;
}

void Core::Socket::connect() {

	fd_set write;
	FD_ZERO(&write);
	FD_SET(socket_, &write);
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	if (select(socket_ + 1, &write, 0, 0, &tv)) {
		if (FD_ISSET(socket_, &write)) {
			type_ = STREAM;
		}
	}
}

void Core::Socket::write_datagram() {
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
	if (rt < 0) {
		if (EWOULDBLOCK == errcode()) {
			return;
		} else {
			throw runtime_error(errmsg());
		}
    } else if (rt != (int)len) {
        throw runtime_error("Failed to send datagram");
	} else {
		write_bytes_ += rt;
	}

	out_.clear();
}

void Core::Socket::read_datagram() {
    sockaddr_in source;
    socklen_t socklen = sizeof(source);

    // Assume the packet will be 4096 bytes, max.
    in_.resize(4096);
    
    // Get the packet pointer and length
    char* pkt = &in_[0];
    size_t len = in_.size();
        
    // Read from the socket, and capture the source addres.
    int rt = recvfrom(socket_, pkt, len, 0, (sockaddr*)&source, &socklen);

    // If an error occurred, or the socket is already closed, then throw an
    // exception.
	if (rt < 0) {
		if (EWOULDBLOCK == errcode()) {
			return;
		} else {
			throw runtime_error(errmsg());
		}
    } else if (rt < (int)sizeof(size_t)) {
        throw runtime_error("Invalid packet");
	} else {
		read_bytes_ += rt;
	}
    
    // Read the length of the packet from the beginning of the packet, as
    // long as there are at least 4 bytes in the packet (size of an int).
    in_.resize(ntohl(*(size_t*)&in_[0]));
}

void Core::Socket::write_stream() {        
    // Write the length of the packet into the header.  This includes the
    // length of the header itself.
    if (!write_bytes_) {
        *(size_t*)&out_[0] = htonl(out_.size());
    }
    
    // Attempt to send any bytes remaining in the buffer.
    char* pkt = &out_[write_bytes_];
    size_t len = out_.size() - write_bytes_;
    
    // Write to the socket
    int rt = send(socket_, pkt, len, 0);
    
    // If an error occurred, or the socket is already closed, then throw
    // an exception
	if (rt < 0) {
		if (EWOULDBLOCK == errcode()) {
			return;
		} else {
			throw runtime_error(errmsg());
		}
    } else {
        write_bytes_ += rt;
    }
    
    //!If the number of sent bytes equals the buffer size, then the packet
    // is done sending.  Thus, we can reset the buffer.
    if (write_bytes_ == out_.size()) {
        out_.clear();
        write_bytes_ = 0;
    }
}

void Core::Socket::read_stream() {
    // If the number of bytes received is zero, then resize buffer
    // to hold 4096 bytes
    if (!read_bytes_) {
        in_.resize(sizeof(size_t));
    }
    
    // Attempt to read the rest of the packet
    char* pkt = &in_[read_bytes_];
    size_t len = in_.size() - read_bytes_;   
    
    // Read from the socket, and capture the source addres.
    int rt = recv(socket_, pkt, len, 0);
    
    // If an error occurred, or the socket is already closed, then throw an
    // exception.
	if (rt < 0) {
		if (EWOULDBLOCK == errcode()) {
			return;
		} else {
			throw runtime_error(errmsg());
		}
    } else if (rt == 0) {
        throw runtime_error("Socket is closed");
    } else {
        read_bytes_ += rt;
    }
    
    //! Check if all the bytes from the packet have been read.
    if (read_bytes_ >= sizeof(size_t)) {
        in_.resize(ntohl(*(size_t*)&in_[0]));
	}
}

Core::SocketWriter* Core::Socket::writer() {
    // Return a socket writer if the buffer is empty.
	poll_write();

    if (out_.empty()) {
        return new Core::SocketWriter(this);
    } else {
        return 0;
    }
}

Core::SocketReader* Core::Socket::reader() {
    // Poll for more data from the socket.
    poll_read();

    // Return a socket reader if the buffer is not empty, and no packet read
    // is currently in progress
    if (!in_.empty() && in_.size() == read_bytes_) {
        return new Core::SocketReader(this);
    } else {
        return 0;
    }
}
