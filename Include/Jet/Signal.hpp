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

#include <Jet/Types.hpp>
#include <boost/any.hpp>

namespace Jet {

//! Represents a signal, which is basically a very primitive RPC with two
//! arguments (which is plenty for most purposes)
//! @class Signal
//! @brief a signal
class Signal {
public:
    //! Creates a new signal with the given name and parameters.
    Signal(const std::string& name, const boost::any& a, const boost::any& b) :
        name(name),
        first(a),
        second(b) {
    }

    //! Creates a new signal with the given name and parameters.
    Signal(const std::string& name, const boost::any& a) :
        name(name),
        first(a) {
    }
    
    //! Creates a new signal with the given name and parameters.
    Signal(const std::string& name) :
        name(name) {
            
    }
    
    const std::string name;
    const boost::any first;
    const boost::any second;
};

}
