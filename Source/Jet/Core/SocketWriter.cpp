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

#include <Jet/Core/SocketWriter.hpp>

using namespace Jet;
using namespace std;

Core::SocketWriter::SocketWriter(Socket* socket) :
    socket_(socket),
    bytes_written_(sizeof(size_t)) {

    socket_->out_.resize(sizeof(size_t));
}

Core::SocketWriter::~SocketWriter() {
    // Send right away if possible.
    socket_->poll_write();
}

void Core::SocketWriter::real(float real) {
    socket_->out_.resize(socket_->out_.size() + sizeof(real));
    
    *(float*)&socket_->out_[bytes_written_] = real;
    bytes_written_ += sizeof(real);
}

void Core::SocketWriter::integer(int integer) {
    socket_->out_.resize(socket_->out_.size() + sizeof(integer));
    
    *(int*)&socket_->out_[bytes_written_] = htonl(integer);
    bytes_written_ += sizeof(integer);
}

void Core::SocketWriter::string(const std::string& string) {
    size_t length = string.length() + 1;
    socket_->out_.resize(socket_->out_.size() + length);
    
    memcpy(&socket_->out_[bytes_written_], string.c_str(), length);
    bytes_written_ += length;
}