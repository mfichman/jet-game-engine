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
#include <stdexcept>

namespace Jet {
using namespace std;

class Transformer {
public:
	virtual ~Transformer() {}
	virtual void operator()(Real& o) { throw logic_error("Invalid type"); }
	virtual void operator()(Integer& o) { throw logic_error("Invalid type"); }
	virtual void operator()(ID& o) { throw logic_error("Invalid type"); }
	virtual void operator()(Point& o) { throw logic_error("Invalid type"); }
	virtual void operator()(Coord& o) { throw logic_error("Invalid type"); }
	virtual void operator()(Color& o) { throw logic_error("Invalid type"); }
	virtual void operator()(Vector& o) { throw logic_error("Invalid type"); }
	virtual void operator()(Quaternion& o) { throw logic_error("Invalid type"); }
	virtual void operator()(Frame& o) { throw logic_error("Invalid type"); }

	virtual Real operator()(Null<Real>) { throw logic_error("Invalid type"); }
	virtual Integer operator()(Null<Integer>) { throw logic_error("Invalid type"); }
	virtual ID operator()(Null<ID>) { throw logic_error("Invalid type"); }
	virtual Point operator()(Null<Point>) { throw logic_error("Invalid type"); }
	virtual Coord operator()(Null<Coord>) { throw logic_error("Invalid type"); }
	virtual Color operator()(Null<Color>) { throw logic_error("Invalid type"); }
	virtual Vector operator()(Null<Vector>) { throw logic_error("Invalid type"); }
	virtual Quaternion operator()(Null<Quaternion>) { throw logic_error("Invalid type"); }
	virtual Frame operator()(Null<Frame>) { throw logic_error("Invalid type"); }

	virtual void typeError() { throw logic_error("Invalid type"); }
};

template <typename T>
class TypedTransformer : public Transformer {
public:
	TypedTransformer(T t) : value_(t) {}

	T value() { return value_; }
	void value(const T& o) { value_ = o; }

	void operator()(T& o) { value_ = o; }
	T operator()(Null<T>) { return value_; }

private:
	T value_;
};

}
