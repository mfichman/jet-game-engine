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
#include <memory>
#include <algorithm>

namespace Jet {


//! This is the base virtual class for an in iterator over an arbitrary
//! container.
//! @class BaseIterator
//! @brief Iterator base class.
template <typename T>
class BaseIterator {
public:
    //! Destructor.
    virtual ~BaseIterator() {}

    //! Returns the current iterator value.
    virtual T value()=0;

    //! Increments the iterator.
    virtual void next()=0;

    //! Returns true if there is a next value.
    virtual bool has_next()=0;

    //! Clones this iterator
    virtual BaseIterator<T>* clone()=0;
};

//! This class is the concrete base class for an iterator over an arbitrary
//! container.
//! @class ConcreteIterator
//! @brief Iterator concrete class.
template <typename T, typename I>
class ConcreteIterator : public BaseIterator<T> {
public:
    //! Constructor
    ConcreteIterator(I begin, I end) :
        begin_(begin),
        end_(end) {
    }

    //! Returns the current iterator value.
    virtual T value() {
        return *begin_;
    }

    //! Increments the iterator.
    virtual void next() {
        begin_++;
    }

    //! Returns true if there is a next value.
    virtual bool has_next() {
        return begin_ != end_;
    }

    //! Clones this iterator
    virtual ConcreteIterator<T, I>* clone() {
        return new ConcreteIterator<T, I>(begin_, end_);
    }
    
private:
    I begin_;
    I end_;
};

//! This class wraps the base class so that both the start and end
//! iterator can be returned through one function call.  In addition, it is
//! specialized to be used with pointers to Jet objects, so that accessing
//! any object from an iterator i is as follows: i->function().
//! @class Iterator
//! @brief Iterator for Jet Objects.
template <typename T>
class Iterator {
public:

    //! Empty iterator constructor.
    Iterator() {
    }

    //! Constructor.  Takes an any_iterator that points to the beginning of a
    //! sequence, and an any_iterator that points to the end of the sequence.
    template <typename I>
    Iterator(I begin, I end) :
        base_(new ConcreteIterator<T&, I>(begin, end)) {
    }

    //! Copy constructor
    Iterator(const Iterator& other) :
        base_(other.base_->clone()) {
    }

    //! Allows the iterator to be used in an if statement.  If the result of
    //! this function is true, the iterator has not reached the end.
    operator bool() {
        return base_.get() && base_->has_next();
    }

    //! Overrides operator-> so that element methods may be accessed directly
    //! from the iterator, as in itr->my_function().
    T* operator->() {
        return &base_->value();
    }
    
    //! Overrides operator*.
    T& operator*() {
        return base_->value();
    }

    //! Increments the iterator. (Prefix)
    void operator++() {
        base_->next();
    }

    //! Increments the iterator. (Postfix)
    void operator++(int) {
        base_->next();
    }

private:
    std::auto_ptr<BaseIterator<T&> > base_;
};

}
