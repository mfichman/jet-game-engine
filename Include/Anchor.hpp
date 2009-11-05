/*
 * Copyright (c) 2008 Matt Fichman
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

#include <Types.hpp>

namespace Jet { 
using namespace std;
using namespace std::tr1;

class Anchor : public InterfaceImpl<Anchor> {
public:
    typedef shared_ptr<Anchor> Ptr;
	typedef shared_ptr<const Anchor> ConstPtr;
	friend class Object;
    
    inline Vector       position() const { return position_; }
    inline void         position(const Vector& v) { modState(modified); position_ = v; }
    inline Quaternion   rotation() const { return rotation_; }
    inline void         rotation(const Quaternion& q) { modState(modified); rotation_ = q; }
	inline Anchor::Ptr	parent() { return parent_; }
	inline void			parent(Anchor::Ptr p) { parent_ = p; }
    
protected:    
	Anchor::Ptr			parent_;
    Vector              position_;
    Quaternion          rotation_;
};

}
