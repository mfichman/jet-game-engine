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

#include <vector>
#include <map>

namespace Jet {
using namespace std;

template <typename T>
class Iterator { 
public:
	Iterator(T& v) : iter_(v.begin()), end_(v.end()) {}
	void operator++() { iter_++; }
	typename T::value_type& operator*() { return *iter_; }
	typename T::value_type& operator->() { return *iter_; }
	operator bool() { return iter_ != end_; }

private:
	typename T::iterator iter_;
	typename T::iterator end_;
};

template <typename K, typename V>
class Iterator<map<K,V> > {
public:
    Iterator(map<K,V>& m) : iter_(m.begin()), end_(m.end()) {}
    void operator++() { iter_++; }
    V& operator*() { return iter_->second; }
    V& operator->() { return iter_->second; }
    operator bool() { return iter_ != end_; }
    
private:
    typename map<K,V>::iterator iter_;
    typename map<K,V>::iterator end_;
};

}
