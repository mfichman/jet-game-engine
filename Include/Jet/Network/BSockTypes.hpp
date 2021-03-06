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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOVT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BVT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AVTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OVT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  
#pragma once

#include <Jet/Types.hpp>
#include <stdexcept>
#include <boost/intrusive_ptr.hpp>
#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
inline const char* socket_errmsg() {
    static char buffer[512];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), NULL, buffer, 512, NULL);
    return buffer;
}    
inline int socket_errcode() {
    return WSAGetLastError();
}
#define EWOULDBLOCK WSAEWOULDBLOCK
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <boost/intrusive_ptr.hpp>
#define SD_BOTH SHUT_RDWR
#define INVALID_SOCKET -1
#define socket_errmsg() strerror(errno)
#define socket_errcode() errno
#define closesocket close
#endif
#undef ST_CLIENT

namespace Jet {

    class BSockGame;
    class BSockPlayer;
	class BSockNetworkMonitor;
    class BSockNetwork;
    class BSockShader;
    class BSockServerSocket;
    class BSockSocket;
    class BSockWriter;
    class BSockReader;
    
	typedef boost::intrusive_ptr<BSockNetworkMonitor> BSockNetworkMonitorPtr;
	typedef boost::intrusive_ptr<BSockNetwork> BSockNetworkPtr;
    typedef boost::intrusive_ptr<BSockServerSocket> BSockServerSocketPtr;
    typedef boost::intrusive_ptr<BSockSocket> BSockSocketPtr;
    typedef boost::intrusive_ptr<BSockReader> BSockReaderPtr;
    typedef boost::intrusive_ptr<BSockWriter> BSockWriterPtr;

    enum SocketType { 
		ST_SERVER, 
		ST_CLIENT, 
		ST_MULTICAST, 
		ST_DATAGRAM, 
		ST_STREAM 
	};

	enum PacketType { 
		PT_PING,
		PT_MATCH_INFO,
		PT_MATCH_DESTROY,
		PT_PLAYER_JOIN,
		PT_PLAYER_LEAVE,
		PT_PLAYER_LIST,
		PT_STATE,
		PT_INPUT,
		PT_SYNC,
		PT_USER_RPC,
	};

	enum DataType {
		DT_NUMBER,
		DT_STRING,
		DT_BOOL,
		DT_NIL
	};
}
