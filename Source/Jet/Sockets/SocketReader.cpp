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

#include <Jet/Sockets/SocketReader.hpp>

using namespace Jet;
using namespace std;

Sockets::SocketReader::SocketReader(Socket* socket) :
    socket_(socket),
    bytes_read_(sizeof(size_t)) {
}

Sockets::SocketReader::~SocketReader() {
    // Clear the buffer so that the next packet can be read.
    socket_->in_.clear();
	socket_->read_bytes_ = 0;
}

float Sockets::SocketReader::real() {
    if (socket_->in_.size() - bytes_read_ < sizeof(float)) {
        throw std::runtime_error("No more data in packet");
    }
    
    float real = *(float*)&socket_->in_[bytes_read_];
    bytes_read_ += sizeof(real);
    
    return real;
}

int Sockets::SocketReader::integer() {
    if (socket_->in_.size() - bytes_read_ < sizeof(int)) {
        throw std::runtime_error("No more data in packet");
    }
    
    int integer = ntohl(*(int*)&socket_->in_[bytes_read_]);
    bytes_read_ += sizeof(integer);
    
    return integer;
}

std::string Sockets::SocketReader::string() {
    if (socket_->in_.size() - bytes_read_ == 0) {
        throw std::runtime_error("No more data in packet");
    }
    
    std::string string(&socket_->in_[bytes_read_], 0, socket_->in_.size() - bytes_read_);
    bytes_read_ += string.length() + 1;
    return string; 
}