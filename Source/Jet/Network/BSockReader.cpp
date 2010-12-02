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

#include <Jet/Network/BSockReader.hpp>
#include <Jet/Types/Quaternion.hpp>

using namespace Jet;
using namespace std;

BSockReader::BSockReader(BSockSocket* socket) :
    socket_(socket),
    bytes_read_(sizeof(size_t)) {

	// Swap our vector with the vector that was read from the socket
	// so that the socket has an empty buffer to work with while we
	// process this buffer
	in_.reserve(2048);
	in_.swap(socket_->in_);
}

BSockReader::~BSockReader() {
    // Clear the buffer so that the next packet can be read.
    socket_->in_.clear();
	socket_->read_bytes_ = 0;
}

Vector BSockReader::vector() {
	Vector vector;
	vector.x = real();
	vector.y = real();
	vector.z = real();
	return vector;
}

Quaternion BSockReader::quaternion() {
	Quaternion quaternion;
	quaternion.w = real();
	quaternion.x = real();
	quaternion.y = real();
	quaternion.z = real();
	return quaternion;
}

float BSockReader::real() {
    if (in_.size() - bytes_read_ < sizeof(float)) {
        throw std::runtime_error("No more data in packet");
    }
    
    float real = *(float*)&in_[bytes_read_];
    bytes_read_ += sizeof(real);
    
    return real;
}

int BSockReader::integer() {
    if (in_.size() - bytes_read_ < sizeof(int)) {
        throw std::runtime_error("No more data in packet");
    }
    
    int integer = ntohl(*(int*)&in_[bytes_read_]);
    bytes_read_ += sizeof(integer);
    
    return integer;
}

uint8_t BSockReader::byte() {
    if (in_.size() - bytes_read_ < sizeof(uint8_t)) {
        throw std::runtime_error("No more data in packet");
    }
    
    uint8_t byte = *(uint8_t*)&in_[bytes_read_];
    bytes_read_ += sizeof(byte);
    
    return byte;
} 

std::string BSockReader::string() {
    if (in_.size() - bytes_read_ == 0) {
        throw std::runtime_error("No more data in packet");
    }
    
    std::string string(&in_[bytes_read_], 0, in_.size() - bytes_read_);
    bytes_read_ += string.length() + 1;
    return string; 
}