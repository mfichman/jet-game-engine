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
#pragma once

#include <Jet/Network/BSockTypes.hpp>
#include <Jet/Network/BSockSocket.hpp>
#include <Jet/Object.hpp>

namespace Jet {

//! Writes data to a socket synchronously.
//! @class BSockWriter
//! @brief Writes data to a socket synchronously.
class BSockWriter : public Object {
public:
    //! Creates a new socket writer, and locks the socket's buffer.
    BSockWriter(BSockSocket* socket);
    
    //! Destructor
    ~BSockWriter();

	//! Writes a vector to the socket.  Does not block.
	void vector(const Vector& vector);

	//! Writes a quaternion to the socket.  Does not block.
	void quaternion(const Quaternion& quaternion);
    
    //! Writes a floating-point number to the socket.  Does not block.
    void real(float real);
    
    //! Writes an integer number to the socket.
    void integer(int integer);

	//! Writes a byte to the socket
	void byte(uint8_t byte);
    
    //! Writes a string to the socket
    void string(const std::string& string);

	//! Writes the whole packet stored in the given reader.
	void packet(BSockReader* reader);

    //! Returns the socket
    inline BSockSocket* socket() const {
        return socket_.get();
    }
    
private:
    BSockSocketPtr socket_;
    size_t bytes_written_;
};

}
