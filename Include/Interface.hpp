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

#include <Transformer.hpp>
#include <Registry.hpp>
#include <string>
 
namespace Jet {
using namespace std;
using namespace std::tr1;

class Interface {
public: 
	typedef shared_ptr<Interface> Ptr;
    enum ModState { modified, unmodified };
    class Listener;
    
    inline ModState     modState() { return modState_; }
    Listener*           listener() { return listener_; }
	inline void         listener(Listener* l) { listener_ = l; }
	void				set(const string& n, Transformer& t) { class_->set(this, n, t); }
	void				get(const string& n, Transformer& t) { class_->set(this, n, t); }

protected:
	Interface(Class::Ptr c) : class_(c), listener_(0) {}
    inline void modState(ModState s);
    
private:
    ModState            modState_;
    Listener*           listener_; 
	Class::Ptr			class_;
};

class Interface::Listener
{
public:
    virtual void        onModState(Interface* obj)=0;
};

inline void
Interface::modState(Interface::ModState s) { 
	if (listener_ && s != modState_) {
		listener_->onModState(this);
	}
	modState_ = s;
}

template <typename T>
class InterfaceImpl : public Interface {
public:
	InterfaceImpl() : Interface(ClassImpl<T>::instance()) {}
};

}
