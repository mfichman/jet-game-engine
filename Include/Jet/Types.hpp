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

#ifdef WINDOWS
#include <memory>
#include <functional>
#else
#include <tr1/memory>
#include <tr1/functional>
#endif
#include <limits>

namespace Jet {
using namespace std;

template <typename B>
class Null {};

class BaseTuple {};

template <typename T, int N>
class Tuple : public BaseTuple {
public:
	static const int size = N;
	typedef T value_type;

	inline T& operator[](int index) {
		if (index < 0 || index > N) {
			throw std::out_of_range("Tuple");
		}
		return values_[index];
	}
	inline void operator=(const Tuple<T, N>& o) {
		copy(o.values_, o.values_ + N, values_);
	}

private:
	T values_[N];
};

class BaseNominal {};

template <typename T>
class Nominal : public BaseNominal {
public:
	typedef T value_type;

	inline Nominal() : value_(0) {}
    inline Nominal(const T& value) : value_(value) {}
    inline T& value() { return value_; }
    inline const T& value() const { return value_; }
    inline void value(T t) { value_ = t; }
    inline bool operator==(const Nominal<T>& o) const { return value_ == o.value_; }
	inline const Nominal& operator=(const Nominal& o) { value_ = o.value_; return *this; }
    inline operator T() { return value_; }
    
protected:
    T value_;
};

template <typename T>
class Ordinal : public Nominal<T>
{
public:
	inline Ordinal() : Nominal<T>(0) {}
    inline Ordinal(const T& value) : Nominal<T>(value) {}
    inline bool operator<(const T& o) const { return value_ < o.value_; }
    inline bool operator>(const T& o) const { return value_ > o.value_; }
};

template <typename T, long MIN, long MAX>
class RangedOrdinal : public Ordinal<T>
{
public:
    inline RangedOrdinal() : Ordinal<T>(0) {}
    inline RangedOrdinal(const T& value) : Ordinal<T>(value) {
        if (value < MIN || value > MAX) {
            throw std::out_of_range("RangedOrdinal");
        }
    }
	static const long maxValue = MAX;
	static const long minValue = MIN;
};


template <typename T>
class Number : public Ordinal<T> 
{
public:
	inline Number() : Ordinal<T>(0) {}
    inline Number(const T& value) : Ordinal<T>(value) {}
    inline T operator+(const T& o) const { return Real(value_ + o.value); }
    inline T operator*(const T& o) const { return Real(value_ * o.value); }
    inline T operator/(const T& o) const { return Real(value_ / o.value); }
    inline T operator-(const T& o) const { return Real(value_ - o.value); }
    inline void operator+=(const T& o) const { value_ += o.value; }
    inline void operator-=(const T& o) const { value_ -= o.value; }
    inline void operator*=(const T& o) const { value_ *= o.value; }
    inline void operator/=(const T& o) const { value_ /= o.value; }
};


class Real : public Number<float> {};
class Integer : public Number<int> {};
class ID : public Nominal<unsigned> {};
class FrameID : public Ordinal<unsigned> {};
class Point : public Tuple<int, 2> {};
class Coord : public Tuple<float, 2> {};
class ScreenCoord : public Ordinal<short> {};
class Color : public Tuple<float, 4> {};
class Vector : public Tuple<float, 3> {};
class Quaternion : public Tuple<float, 4> {};
class FontPoint : public RangedOrdinal<short, 4, 64> {};
class Dimension : public Tuple<short, 2> {};
class Vertex : public Tuple<int, 2> {};

struct Frame
{
    FrameID frameId_;
    float time_;
    float delta_;
};

}
