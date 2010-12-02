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

#include <Jet/Network/BSockWriter.hpp>
#include <Jet/Network/BSockReader.hpp>
#include <Jet/Types/Quaternion.hpp>

using namespace Jet;
using namespace std;

BSockWriter::BSockWriter(BSockSocket* socket) :
    socket_(socket),
    bytes_written_(sizeof(size_t)) {

	socket_->out_.push(std::vector<char>());
    socket_->out_.back().resize(sizeof(size_t));
}

BSockWriter::~BSockWriter() {
    // Send right away if possible.
    socket_->poll_write();
}

void BSockWriter::quaternion(const Quaternion& quaternion) {
	real(quaternion.w);
	real(quaternion.x);
	real(quaternion.y);
	real(quaternion.z);
}

void BSockWriter::vector(const Vector& vector) {
	real(vector.x);
	real(vector.y);
	real(vector.z);
}

void BSockWriter::real(float real) {
	std::vector<char>& out = socket_->out_.back();
    
    out.resize(out.size() + sizeof(real));
    *(float*)&out[bytes_written_] = real;
    bytes_written_ += sizeof(real);
}

void BSockWriter::integer(int integer) {
	std::vector<char>& out = socket_->out_.back();
        
    out.resize(out.size() + sizeof(integer));
    *(int*)&out[bytes_written_] = htonl(integer);
    bytes_written_ += sizeof(integer);
}

void BSockWriter::byte(uint8_t byte) {
	std::vector<char>& out = socket_->out_.back();
        
    out.resize(out.size() + sizeof(byte));
    *(uint8_t*)&out[bytes_written_] = byte;
    bytes_written_ += sizeof(byte);
}

void BSockWriter::string(const std::string& string) {
	std::vector<char>& out = socket_->out_.back();
    
    size_t length = string.length() + 1;
    out.resize(out.size() + length);
    
    memcpy(&out[bytes_written_], string.c_str(), length);
    bytes_written_ += length;
}

void BSockWriter::packet(BSockReader* reader) {
	std::vector<char>& out = socket_->out_.back();

	size_t length = reader->in_.size();
	out.resize(out.size() + length);

	copy(reader->in_.begin(), reader->in_.end(), out.begin() + bytes_written_);
	bytes_written_ += length;
}	