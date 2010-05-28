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

#include <Jet/Network/BSockServerSocket.hpp>
#include <Jet/Network/BSockSocket.hpp>

using namespace Jet;
using namespace std;

BSockServerSocket* BSockServerSocket::server(const Address& address) {
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY); // Choose any address
    local.sin_port = htons(address.port); // Choose any port for TCP

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
    remote.sin_port = 0;
    
    return new BSockServerSocket(local, remote);
}

BSockServerSocket::BSockServerSocket(const sockaddr_in& local, const sockaddr_in& remote) :
	local_(local),
	remote_(remote) {

    // Create a new TCP socket.  If the attempt fails, throw an exception.
    socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == socket_) {
        throw runtime_error(socket_errmsg());
    }
    
    // Attempt to bind the socket to any port.  If the attempt fails,
    // clean close the socket and throw an exception.
    if (bind(socket_, (sockaddr*)&local_, sizeof(local_)) < 0) {
        throw runtime_error(socket_errmsg());
    }

	// Listen for connections
	if (listen(socket_, 1) < 0) {
		throw runtime_error(socket_errmsg());
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
    
    // Look up the local IP address
    uint16_t port = local_.sin_port;
    char hostname[255];
    gethostname(hostname, 255);
    hostent* entry;
    entry = gethostbyname(hostname);
    local_.sin_addr = *(in_addr*)*entry->h_addr_list;
    local_.sin_port = port;
    
    address_.address = ntohl(local_.sin_addr.s_addr);
    address_.port = ntohs(local_.sin_port);
}

BSockServerSocket::~BSockServerSocket() {
    if (socket_ != INVALID_SOCKET) { 
        shutdown(socket_, SD_BOTH);
        closesocket(socket_);
    }
}

BSockSocket* BSockServerSocket::socket() {
    // Attempt to accept a socket

    socklen_t socklen = sizeof(remote_);
	int sd = ::accept(socket_, (sockaddr*)&remote_, &socklen);
	if (INVALID_SOCKET == sd) {
		if (EWOULDBLOCK == socket_errcode()) {
			return 0;
		} else {
			throw runtime_error(socket_errmsg());
		}
    }
    
    return new BSockSocket(local_, remote_, ST_STREAM, sd);
}

