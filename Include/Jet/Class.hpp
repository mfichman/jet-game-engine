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
#include <Jet/Source.hpp>
#include <Jet/Sink.hpp>
#include <map>
#include <string>
#include <iostream>
#pragma warning(disable:4251)

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;
using namespace boost;

class Interface;
class Source;
class Sink;

class JETAPI Class {
public:
    typedef function<void (intrusive_ptr<Interface>, Source&)>  Mutator;
    typedef function<void (intrusive_ptr<Interface>, Sink&)>    Accessor;
    typedef std::tr1::shared_ptr<Class> Ptr;


    inline void             mutator(const string& n, Mutator m) { mutator_[n] = m; }
    inline const Mutator&   mutator(const string& n) { return mutator_[n]; }
    inline void             accessor(const string& n, Accessor m) { accessor_[n] = m; }
    inline const Accessor&  accessor(const string& n) { return accessor_[n]; }
    
    inline const map<string, Mutator>   mutators() { return mutator_; }
    inline const map<string, Accessor>  accessors() { return accessor_; }
   
    template <typename C, typename A>
    inline void mutator(const string& n, void (C::*f)(A));
    
    template <typename C, typename A>
    inline void accessor(const string& n, A (C::*f)(void) const);
    
private:
    
    map<string, Mutator>    mutator_;
    map<string, Accessor>   accessor_;
};


template <typename C, typename A>
class MutatorFunctor {
public:
    typedef void (C::*Function)(A);
    typedef void result_type;
    
    MutatorFunctor(Function f) : f_(f) {}
    void operator()(intrusive_ptr<Interface> i, Source& s) const;
    
private:
    Function f_;
};

template <typename C, typename A>
class MutatorFunctor<C, const A&> {
public:
    typedef void (C::*Function)(const A&);
    typedef void result_type;
    
    MutatorFunctor(Function f) : f_(f) {}
    void operator()(intrusive_ptr<Interface> i, Source& s) const;
    
private:
    Function f_;
};

template <typename C, typename A>
class AccessorFunctor {
public:
    typedef A (C::*Function)(void) const;
    typedef void result_type;

    AccessorFunctor(Function f) : f_(f) {} 
    void operator()(intrusive_ptr<Interface> i, Sink& s) const;
    
private:
    Function f_;
};

template <typename C, typename A>
inline void
Class::mutator(const string& n, void (C::*f)(A)) {
    mutator(n, bind(MutatorFunctor<C,A>(f), _1, _2));
}
    
template <typename C, typename A>
inline void 
Class::accessor(const string& n, A (C::*f)(void) const) {
    accessor(n, bind(AccessorFunctor<C,A>(f), _1, _2));
}

template <typename C, typename A>
void 
MutatorFunctor<C, A>::operator()(intrusive_ptr<Interface> i, Source& s) const {
    C* obj = dynamic_cast<C*>(i.get());
    if (obj) {
        A a;
        s(a);
        ((*obj).*f_)(a);
    } else {
        cout << "Invalid type, expected \"" << typeid(C).name();
        cout << "\" but found \"" << typeid(*i.get()).name() << "\"" << endl;
    }
}

template <typename C, typename A>
void 
MutatorFunctor<C, const A&>::operator()(intrusive_ptr<Interface> i, Source& s) const {
    C* obj = dynamic_cast<C*>(i.get());
    if (obj) {
        A a;
        s(a);
        ((*obj).*f_)(a);
    } else {
        cout << "Invalid type, expected \"" << typeid(C).name();
        cout << "\" but found \"" << typeid(*i.get()).name() << "\"" << endl;
    }
}

template <typename C, typename A>
void
AccessorFunctor<C, A>::operator()(intrusive_ptr<Interface> i, Sink& s) const {
    C* obj = dynamic_cast<C*>(i.get());
    if (obj) {
        //s(((*obj).*f_)());
        A a = ((*obj).*f_)();
        s(a);
    } else {
        cout << "Invalid type, expected \"" << typeid(C).name();
        cout << "\" but found \"" << typeid(*i.get()).name() << "\"" << endl;
    }
}



}
