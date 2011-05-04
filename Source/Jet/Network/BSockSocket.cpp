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

#include <Jet/Network/BSockSocket.hpp>
#include <Jet/Network/BSockWriter.hpp>
#include <Jet/Network/BSockReader.hpp>
#include <Jet/Network/BSockNetwork.hpp>
#include <stdexcept>
#include <cstring>
#include <iostream>

using namespace Jet;
using namespace std;

BSockSocket* BSockSocket::server(CoreEngine* engine, const Address& address) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = htons(address.port); // Choose any port for TCP

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = 0;
    
    return new BSockSocket(engine, local, remote, ST_SERVER);
}

BSockSocket* BSockSocket::client(CoreEngine* engine, const Address& address) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = 0; // Choose any port for TCP

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = htonl(address.address);
    remote.sin_port = htons(address.port);
    
    return new BSockSocket(engine, local, remote, ST_CLIENT);

}

BSockSocket* BSockSocket::multicast(CoreEngine* engine, const Address& address) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = htons(address.port); // Bind local to mcast port

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = htonl(address.address);
    remote.sin_port = htons(address.port);
    
    return new BSockSocket(engine, local, remote, ST_MULTICAST);

}

BSockSocket* BSockSocket::datagram(CoreEngine* engine, const Address& address) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = htons(address.port); // Bind local to any given port

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = 0;
    remote.sin_port = 0;
    
    return new BSockSocket(engine, local, remote, ST_DATAGRAM);
}

BSockSocket::BSockSocket(CoreEngine* engine, const sockaddr_in& local, const sockaddr_in& remote, SocketType type, int socket) :
    engine_(engine),
	socket_(socket),
    local_(local),
    remote_(remote),
	type_(type),
    write_bytes_(0),
    read_bytes_(0) {
        
    switch (type_) {
        case ST_DATAGRAM: init_datagram(); break;
        case ST_MULTICAST: init_multicast(); break;
        case ST_SERVER: init_server(); break;
        case ST_CLIENT: init_client(); break;
        default: return;
    }
    
#ifdef WINDOWS
    // Use non-blocking sockets
    u_long yes = 1;
    if (ioctlsocket(socket_, FIONBIO, &yes) < 0) {
        throw runtime_error(socket_errmsg());
    }
#else
	int flags = fcntl(socket_, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(socket_, F_SETFL, flags);
#endif

    // Get the port we are bound to
    socklen_t len = sizeof(local_);
    if (getsockname(socket_, (sockaddr*)&local_, &len) < 0) {
        throw runtime_error(socket_errmsg());
    }
    
	if (ST_MULTICAST == type_) {
		remote_.sin_port = local_.sin_port;
		address_.address = ntohl(remote_.sin_addr.s_addr);
	} else {
		address_.address = ntohl(local_.sin_addr.s_addr);
	}
    address_.port = ntohs(local_.sin_port);
}

BSockSocket::~BSockSocket() {	
    if (socket_ != INVALID_SOCKET) {
#ifdef WINDOWS
		shutdown(socket_, SD_BOTH);
#endif
        closesocket(socket_);
    }
}

void BSockSocket::init_server() {
    // Create a new TCP socket.  If the attempt fails, throw an exception.
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(socket_errmsg());
    }
    
    // Attempt to bind the socket to any port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(::bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(socket_errmsg());
    }

	// Listen for connections
	if (::listen(socket_, 1) < 0) {
		throw runtime_error(socket_errmsg());
	}
}

void BSockSocket::init_client() {    
    // Create a new TCP socket.  If the attempt fails, throw an exception.
    socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(socket_errmsg());
    }
    
    // Attempt to bind the socket to any port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(::bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(socket_errmsg());
    }

	// Attempt to bind the socket to the given port.  This is non-blocking,
    // so the socket will return EWOULDBLOCK.  If an error would have happened
    // for a blocking connect, then we will catch that error on the first read
    // or write.
    if (::connect(socket_, (sockaddr*)&remote_, sizeof(remote_)) < 0) {
        if (EWOULDBLOCK != socket_errcode()) {
            throw runtime_error(socket_errmsg());
        }
    }
}

void BSockSocket::init_multicast() {    
    ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = remote_.sin_addr.s_addr;
    mreq.imr_interface.s_addr = local_.sin_addr.s_addr;
    
    // Create a new UDP socket.  If the attempt fails, throw an exception.
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(socket_errmsg());
    }
    

    int yes = 1;
#ifdef SO_REUSEPORT
	// OS X hack...for some reason, this needs to be set in OS X only
	if (::setsockopt(socket_, SOL_SOCKET, SO_REUSEPORT, (char*)&yes, sizeof(yes)) < 0) {
		throw runtime_error(socket_errmsg());
	}
#endif

    // Set the port to be reusable.  If the attempt fails, close the socket
    // and throw an exception.
    if (::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) < 0) {
        throw runtime_error(socket_errmsg());
    }
    
    // Join the multicast group.  If the attempt fails, close the socket and
    // throw and exception.
    if (::setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(ip_mreq)) < 0) {
        throw runtime_error(socket_errmsg());
    }
    
    // Attempt to bind the socket to the given port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(::bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(socket_errmsg());
    }

	// Get the port we are bound to
    socklen_t len = sizeof(local_);
    if (::getsockname(socket_, (sockaddr*)&local_, &len) < 0) {
        throw runtime_error(socket_errmsg());
    }
}

void BSockSocket::init_datagram() {
    
    // Create a new UP socket.  If the attempt fails, throw an exception.
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(socket_errmsg());
    }
    
    // Set the port to be reusable.  If the attempt fails, close the socket
    // and throw an exception.
    int yes = 1;
    if (::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) < 0) {
        throw runtime_error(socket_errmsg());
    }
    
    // Attempt to bind the socket to the given port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if(::bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(socket_errmsg());
    }
}

void BSockSocket::poll_read() {
    
    // Attempt to accept the incoming connection
    if (ST_SERVER == type_) {
        accept();
    } else if (ST_CLIENT == type_) {
        connect();
    }
    
    // If the input buffer is empty or a read is in progress, then continue
    // reading the packet if data is available.
    if (in_.empty() || in_.size() != read_bytes_) {
        if (ST_MULTICAST == type_ || ST_DATAGRAM == type_) {
            read_datagram();
        } else if (ST_STREAM == type_) {
            read_stream();
        }
    }
}

void BSockSocket::poll_write() {
    // Attempt to accept the incoming connection
    if (ST_SERVER == type_) {
        accept();
    } else if (ST_CLIENT == type_) {
        connect();
    }
    
    // Get the current queue length
    size_t queue_length = out_.size();
    
    // If the output buffer is not empty, then continue writing the packet
    // if writing is permissible.
    while (!out_.empty()) {
        if (ST_MULTICAST == type_ || ST_DATAGRAM == type_) {
            write_datagram();
        } else if (ST_STREAM == type_) {
            write_stream();
        }
        
        // Failed to send a packet, so we will break and wait for the next
        // poll to happen
        if (out_.size() == queue_length) {
            break;
        }
    } 
}

void BSockSocket::accept() {
    // Attempt to accept a socket

    socklen_t socklen = sizeof(remote_);
	int sd = ::accept(socket_, (sockaddr*)&remote_, &socklen);
	if (INVALID_SOCKET == sd) {
		if (EWOULDBLOCK == socket_errcode()) {
			return;
		} else {
			throw runtime_error(socket_errmsg());
		}
    }
    
    // Shut down the server socket, and start using the connection socket.
    shutdown(socket_, SD_BOTH);
    closesocket(socket_);
    socket_ = sd;
    type_ = ST_STREAM;
}

void BSockSocket::connect() {

	fd_set write;
	FD_ZERO(&write);
	FD_SET(socket_, &write);
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select(socket_ + 1, 0, &write, 0, &tv)) {
		if (FD_ISSET(socket_, &write)) {
			type_ = ST_STREAM;
		}
	}
}

void BSockSocket::write_datagram() {
    int socklen = sizeof(remote_);
    vector<char>& out = out_.front();
    
    // Write the length of the packet into the header.  This includes the
    // length of the header itself.
    *(size_t*)&out[0] = htonl(out.size());
    
    // Get the packet pointer and length
    char* pkt = &out[0];
    size_t len = out.size();
        
    // Write to the socket
    int rt = sendto(socket_, pkt, len, 0, (sockaddr*)&remote_, socklen);
        
    // If an error occurred, or the socket is already closed, then throw
    // an exception
	if (rt < 0) {
		if (EWOULDBLOCK == socket_errcode()) {
			return;
		} else {
			throw runtime_error(socket_errmsg());
		}
    } else if (rt != (int)len) {
        throw runtime_error("Failed to send datagram");
	}

	BSockNetwork* network = static_cast<BSockNetwork*>(engine_->network());
	network->tx_bytes_ += rt;

    // Remove the packet we just sent from the queue
    out_.pop();
}

void BSockSocket::read_datagram() {
	sockaddr_in address;
    socklen_t socklen = sizeof(address);

    // Assume the packet will be 4096 bytes, max.
    in_.resize(4096);
    
    // Get the packet pointer and length
    char* pkt = &in_[0];
    size_t len = in_.size();
        
    // Read from the socket, and capture the source addres.
    int rt = recvfrom(socket_, pkt, len, 0, (sockaddr*)&address, &socklen);

    // If an error occurred, or the socket is already closed, then throw an
    // exception.
	if (rt < 0) {
		if (EWOULDBLOCK == socket_errcode()) {
			return;
		} else {
			throw runtime_error(socket_errmsg());
		}
    } else if (rt < (int)sizeof(size_t)) {
        throw runtime_error("Invalid packet");
	} else {
		read_bytes_ += rt;
	}

	BSockNetwork* network = static_cast<BSockNetwork*>(engine_->network());
	network->rx_bytes_ += rt;
    
    // Read the length of the packet from the beginning of the packet, as
    // long as there are at least 4 bytes in the packet (size of an int).
    in_.resize(ntohl(*(size_t*)&in_[0]));
}

void BSockSocket::write_stream() {
    vector<char>& out = out_.front();
        
    // Write the length of the packet into the header.  This includes the
    // length of the header itself.
    if (!write_bytes_) {
        *(size_t*)&out[0] = htonl(out.size());
    }
    
    // Attempt to send any bytes remaining in the buffer.
    char* pkt = &out[write_bytes_];
    size_t len = out.size() - write_bytes_;
    
    // Write to the socket
    int rt = send(socket_, pkt, len, 0);
    
    // If an error occurred, or the socket is already closed, then throw
    // an exception
	if (rt < 0) {
		if (EWOULDBLOCK == socket_errcode()) {
			return;
		} else {
			throw runtime_error(socket_errmsg());
		}
    } else {
        write_bytes_ += rt;
    }

	BSockNetwork* network = static_cast<BSockNetwork*>(engine_->network());
	network->tx_bytes_ += rt;
    
    //!If the number of sent bytes equals the buffer size, then the packet
    // is done sending.  Thus, we can reset the buffer.
    if (write_bytes_ == out.size()) {
        out_.pop();
        write_bytes_ = 0;
    }
}

void BSockSocket::read_stream() {
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
		if (EWOULDBLOCK == socket_errcode()) {
			return;
		} else {
			throw runtime_error(socket_errmsg());
		}
    } else if (rt == 0) {
        throw runtime_error("BSockSocket is closed");
    } else {
        read_bytes_ += rt;
    }

	BSockNetwork* network = static_cast<BSockNetwork*>(engine_->network());
	network->rx_bytes_ += rt;
    
    //! Check if all the bytes from the packet have been read.
    if (read_bytes_ >= sizeof(size_t)) {
        in_.resize(ntohl(*(size_t*)&in_[0]));
	}
}

BSockWriter* BSockSocket::writer() {
    // Return a socket writer if the buffer is empty.
	poll_write();

    // We can write to the socket if we're using a stream socket (reliable,
    // so it always queues the messages) or if we're using a UDP socket
    // (no queues to minimize latency, plus a UDP packet that doesn't get
    // sent is no big deal...just drop it)
    if (out_.empty() || ST_STREAM == type_) {
        return new BSockWriter(this);
    } else {
        return 0;
    }
}

BSockReader* BSockSocket::reader() {
    // Poll for more data from the socket.
    poll_read();

    // Return a socket reader if the buffer is not empty, and no packet read
    // is currently in progress
    if (!in_.empty() && in_.size() == read_bytes_) {
        return new BSockReader(this);
    } else {
        return 0;
    }
}
