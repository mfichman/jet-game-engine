/*
 * Copyright (c) 2009 Matt Fichman
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
#include <list>
#include <algorithm>
#pragma warning(disable:4251)

namespace Jet {
using namespace std;
using namespace std::tr1;

template <typename T>
class Publisher {
public:   
    // Utility
    void observerAdd(T* l);
    void observerDel(T* l);
    void notify(void (T::*funct)(void));

    template <typename V>
    void notify(void (T::*funct)(V), V v);

private:
    list<T*> observer_;
};

template <typename T>
void
Publisher<T>::observerAdd(T* l) {
    observer_.push_back(l);
}

template <typename T>
void
Publisher<T>::observerDel(T* l) {
    observer_.erase(find(observer_.begin(), observer_.end(), l));
}

template <typename T>
void
Publisher<T>::notify(void (T::*funct)(void)) {
    //(object_->*funct)(void);
    for (typename list<T*>::iterator i = observer_.begin(); i != observer_.end(); i++) {
        ((*i)->*funct)();
    }
}

template <typename T>
template <typename V>
void
Publisher<T>::notify(void (T::*funct)(V), V v) {
    for (typename list<T*>::iterator i = observer_.begin(); i != observer_.end(); i++) {
        ((*i)->*funct)(v);
    }
}

}
