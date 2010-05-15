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

#include <Jet/Core/SocketReader.hpp>

using namespace Jet;
using namespace std;

Core::SocketReader::SocketReader(Socket* socket) :
    socket_(socket),
    lock_(socket->in_mutex_),
    bytes_read_(sizeof(size_t)) {
        
    if (!socket_->in_packet_) {
        throw std::runtime_error("No packet available");
    }
}

Core::SocketReader::~SocketReader() {
    socket_->in_packet_ = false;
    socket_->in_condition_.notify_all();
}

float Core::SocketReader::real() {
    if (socket_->in_.size() - bytes_read_ < sizeof(float)) {
        throw std::runtime_error("No more data in packet");
    }
    
    float real = *(float*)&socket_->in_[bytes_read_];
    bytes_read_ += sizeof(real);
    
    return real;
}

int Core::SocketReader::integer() {
    if (socket_->in_.size() - bytes_read_ < sizeof(int)) {
        throw std::runtime_error("No more data in packet");
    }
    
    int integer = *(int*)&socket_->in_[bytes_read_];
    bytes_read_ += sizeof(integer);
    
    return integer;
}

std::string Core::SocketReader::string() {
    if (socket_->in_.size() - bytes_read_ == 0) {
        throw std::runtime_error("No more data in packet");
    }
    
    std::string string(&socket_->in_[bytes_read_], 0, socket_->in_.size() - bytes_read_);
    bytes_read_ += string.length() + 1;
    return string; 
}