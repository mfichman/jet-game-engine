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

#include <Jet/Jet.hpp>
#include <any_iterator/any_iterator.hpp>

namespace Jet {
using namespace IteratorTypeErasure;

//! This class wraps the any_iterator class so that both the start and end
//! iterator can be returned through one function call.  In addition, it is
//! specialized to be used with pointers to Jet objects, so that accessing
//! any object from an iterator i is as follows: i->function().
//! @class Iterator
//! @brief Iterator for Jet Objects.
template <typename T>
class Iterator {
public:

    //! Constructor.  Takes an any_iterator that points to the beginning of a
    //! sequence, and an any_iterator that points to the end of the sequence.
    Iterator(any_iterator<T*> begin, any_iterator<T*> end) : 
        begin_(begin),
        end_(end) {
    }

    //! Allows the iterator to be used in an if statement.  If the result of
    //! this function is true, the iterator has not reached the end.
    bool bool() {
        return begin_ != end_;
    }

    //! Overrides operator-> so that element methods may be accessed directly
    //! from the iterator, as in itr->my_function().
    T* operator->() {
        return *begin_;
    }

    //! Increments the iterator.
    void operator++() {
        return ++begin_;
    }

    //! Increments the iterator.
    void operator++(int) {
        return begin_++;
    }

private:
    any_iterator<T*> begin_;
    any_iterator<T*> end_;
};

}
