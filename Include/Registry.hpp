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
#include <Transformer.hpp>
#include <map>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;

class Interface;

class Class {
public:
	typedef shared_ptr<Class> Ptr;
	virtual ~Class() {}
	virtual void set(Interface* o, const string& n, Transformer& t)=0;
	virtual void get(Interface* o, const string& n, Transformer& t)=0;
};

template <typename T>
class ClassImpl : public Class {
public:
	typedef shared_ptr<ClassImpl> Ptr;
	typedef typename function<void (T*, Transformer&)> Mutator;
	typedef typename function<void (T*, Transformer&)> Accessor;

	template <typename A>
	inline void
	mutator(const string& n, void (T::*m)(const A&)) { mutator_[n] = bind(&ClassImpl::setter<A>, m, _1, _2); }

	template <typename A>
	inline void
	accessor(const string& n, A (T::*m)(void) const) { accessor_[n] = bind(&ClassImpl::getter<A>, m, _1, _2); }

	static Ptr instance() { static Ptr p(new ClassImpl()); return p; }

	template <typename A>
	static void setter(void (T::*m)(const A&), T* o, Transformer& t) {
		(o->*m)(t(Null<A>()));
	}

	template <typename A>
	static void getter(A (T::*m)(void) const, T* o, Transformer& t) {
		t((o->*m)());
	}

	void set(Interface* o, const string& n, Transformer& t) { mutator_[n](static_cast<T*>(o), t); }
	void get(Interface* o, const string& n, Transformer& t) { accessor_[n](static_cast<T*>(o), t); }

	ClassImpl() {}
	map<string, Mutator>	mutator_;
	map<string, Accessor>	accessor_;
};

class Registry {
public:
	typedef shared_ptr<Registry> Ptr;

	template <typename T, typename A>
	inline void
	mutator(const string& n, void (T::*m)(const A&)) {
		ClassImpl<T>::instance()->mutator(n, m);
	}

	template <typename T, typename A>
	inline void
	accessor(const string& n, A (T::*m)(void) const) {
		ClassImpl<T>::instance()->accessor(n, m);
	}

	static Ptr instance() { static Ptr p(new Registry()); return p; }

private:
	


};

}