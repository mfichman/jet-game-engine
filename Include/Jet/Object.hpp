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

namespace Jet {

//! This is the base class for all Jet Engine objects.  It is used for 
//! reflection and smart pointer reference counting.
class Object {
public:

    //! Constructor.
    Object() : refcount_(0) {}

    //! Destructor
    virtual ~Object() {}

    //! Returns the number of references held on this object.
    inline size_t refcount() const { 
        return refcount_; 
    }

    //! Increments the reference count on this object.
    inline void refcount_inc() const { 
        refcount_++; 
    }

    //! Decrements the reference count on this object.
    inline void refcount_dec() const { 
        refcount_--; 
        if (refcount_ == 0) {
            delete this;
        } 
    }

    //! Clones this object
    virtual Object* clone() { return 0; }

private:
    mutable size_t refcount_;
};

}

inline void intrusive_ptr_add_ref(Jet::Object* object) {
    object->refcount_inc();
}

inline void intrusive_ptr_release(Jet::Object* object) {
    object->refcount_dec();
}
