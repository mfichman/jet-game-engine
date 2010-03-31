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
#include <iostream>

JETAPI std::ostream& operator<<(std::ostream& out, const Jet::Texcoord& texcoord);
JETAPI std::istream& operator>>(std::istream& in, Jet::Texcoord& texcoord);

namespace Jet {

//! Represents a 3-component texcoord.
//! @class Texcoord
//! @brief 3-component texcoord.
class JETAPI Texcoord {
public:
    //! Creates a new texcoord with the given components.
    //! @param u u component
    //! @param v v component
    Texcoord(real_t u, real_t v);

    //! Creates a new vector.
    Texcoord();
    
    //! Stream operator.
    friend std::ostream& ::operator<<(std::ostream& out, const Texcoord& texcoord);

    //! Stream operator.
    friend std::istream& ::operator>>(std::istream& in, Texcoord& texcoord);

    bool operator<(const Texcoord& other) const {
        if (u < other.u) return true;
        return v < other.v;       
    }

    real_t u;
    real_t v;
};

}
