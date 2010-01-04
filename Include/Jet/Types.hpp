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

#include <Jet/Export.hpp>
#include <boost/intrusive_ptr.hpp>
#ifdef WINDOWS
#include <memory>
#include <functional>
#else
#include <tr1/memory>
#include <tr1/functional>
#endif
#include <limits>
#include <stdexcept>
#include <algorithm>

#pragma warning(disable: 4251)

namespace Jet {
using namespace std;
using namespace boost;

class Interface;
JETAPI void intrusive_ptr_add_ref(Interface* t);
JETAPI void intrusive_ptr_release(Interface* t);

class Object;
JETAPI void intrusive_ptr_add_ref(Object* t);
JETAPI void intrusive_ptr_release(Object* t);

//------------------------------------------------------------------------------
template <typename T, int N>
class Tuple {
public:
	static const int size = N;
	typedef T value_type;

	inline Tuple();
	inline const T& operator[](int index) const;
    inline T& operator[](int index);
	inline void operator=(const Tuple<T, N>& o);
	inline bool operator==(const Tuple<T, N>& o) const;
	inline bool operator!=(const Tuple<T, N>& o) const;
    inline void operator+=(const Tuple<T, N>& o);

protected:
	T values_[N];
};

//------------------------------------------------------------------------------
template <typename T>
class JETAPI Nominal {
public:
	typedef T value_type;

	inline Nominal() : value_(0) {}
    inline Nominal(const T& value) : value_(value) {}
    inline T& value() { return value_; }
    inline const T& value() const { return value_; }
    inline void value(T t) { value_ = t; }
    inline bool operator==(const Nominal<T>& o) const { return value_ == o.value_; }
    inline bool operator!=(const Nominal<T>& o) const { return value_ != o.value_; }
	inline const Nominal& operator=(const Nominal& o) { value_ = o.value_; return *this; }
    inline operator T() { return value_; }
    
protected:
    T value_;
};

//------------------------------------------------------------------------------
template <typename T>
class JETAPI Ordinal : public Nominal<T> {
public:
	inline Ordinal() : Nominal<T>(0) {}
    inline Ordinal(const T& value) : Nominal<T>(value) {}
    inline bool operator<(const Ordinal& o) const { return this->value_ < o.value_; }
    inline bool operator>(const Ordinal& o) const { return this->value_ > o.value_; }
};

//------------------------------------------------------------------------------
template <typename T, long MIN, long MAX>
class JETAPI RangedOrdinal : public Ordinal<T> {
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

//------------------------------------------------------------------------------
template <typename T>
class JETAPI Number : public Ordinal<T> {
public:
	inline Number() : Ordinal<T>(0) {}
    inline Number(const T& value) : Ordinal<T>(value) {}
    inline T operator+(const T& o) const { return Real(this->value_ + o.value); }
    inline T operator*(const T& o) const { return Real(this->value_ * o.value); }
    inline T operator/(const T& o) const { return Real(this->value_ / o.value); }
    inline T operator-(const T& o) const { return Real(this->value_ - o.value); }
    inline void operator+=(const T& o) const { this->value_ += o.value; }
    inline void operator-=(const T& o) const { this->value_ -= o.value; }
    inline void operator*=(const T& o) const { this->value_ *= o.value; }
    inline void operator/=(const T& o) const { this->value_ /= o.value; }
};

//------------------------------------------------------------------------------
class JETAPI Real : public Number<float> {
public:
    inline Real(float x) : Number(x) {}
    inline Real() : Number<float>() {}
    inline Real(const Real& x) : Number(x) {}
};

//------------------------------------------------------------------------------
class JETAPI ID : public Nominal<unsigned> {
public:
    inline ID(unsigned x) : Nominal(x) {}
    inline ID(const ID& x) : Nominal(x.value_) {}
};

//------------------------------------------------------------------------------
class JETAPI FontPoint : public RangedOrdinal<short, 4, 64> {
public:
    inline FontPoint(short p) : RangedOrdinal(p) {}
    inline FontPoint(const FontPoint& x) : RangedOrdinal(x.value_) {}
};

//------------------------------------------------------------------------------
class JETAPI Point : public Tuple<int, 2> {
public:
    inline Point(int x, int y) {
        values_[0] = x;
        values_[1] = y;
    }
    inline int x() const { return values_[0]; }
    inline int y() const { return values_[1]; }
};

//------------------------------------------------------------------------------
class JETAPI Color : public Tuple<float, 4> {
public:
    inline Color(float r, float g, float b, float a) {
        values_[0] = r;
        values_[1] = g;
        values_[2] = b;
        values_[3] = a;
    }
    inline Color() {}
    inline float r() const { return values_[0]; }
    inline float g() const { return values_[1]; }
    inline float b() const { return values_[2]; }
    inline float a() const { return values_[3]; }
};

//------------------------------------------------------------------------------
class JETAPI Vector : public Tuple<float, 3> {
public:
    inline Vector(float x, float y, float z) {
        values_[0] = x;
        values_[1] = y;
        values_[2] = z;
    }
    inline Vector() {}
    inline float x() const { return values_[0]; }
    inline float y() const { return values_[1]; }
    inline float z() const { return values_[2]; }
};

//------------------------------------------------------------------------------
class JETAPI Quaternion : public Tuple<float, 4> {
public:
    inline Quaternion(float x, float y, float z, float w) {
        values_[0] = x;
        values_[1] = y;
        values_[2] = w;
        values_[3] = z;
    }
    inline Quaternion() {}
    inline float x() const { return values_[0]; }
    inline float y() const { return values_[1]; }
    inline float z() const { return values_[2]; }
    inline float w() const { return values_[3]; }
};

//------------------------------------------------------------------------------
class JETAPI Dimension : public Tuple<short, 2> {
public:
    inline Dimension(int width, int height) {
        values_[0] = width;
        values_[1] = height;
    }
    inline int width() const { return values_[0]; }
    inline int height() const { return values_[1]; }
};

//------------------------------------------------------------------------------
class JETAPI TexCoord : public Tuple<float, 2> {
public:
    inline TexCoord(float u, float v) {
        values_[0] = u;
        values_[1] = v;
    }
    inline TexCoord() {}
    inline float u() const { return values_[0]; }
    inline float v() const { return values_[1]; }
};

//------------------------------------------------------------------------------
class JETAPI Box {
public:
    inline Box(const Vector& p, float w, float h, float d) :
        position_(p),
        width_(w),
        height_(h),
        depth_(d) {
    }
    inline Box() : width_(0.0f), height_(0.0f), depth_(0.0f) {}
    inline bool operator==(const Box& r) const;
    inline bool operator!=(const Box& r) const;

    Vector position_;
    float width_;
    float height_;
    float depth_;
};

//------------------------------------------------------------------------------
struct JETAPI Sphere {
public:
    inline Sphere(const Vector& p, float r) :
        position_(p),
        radius_(r) {
    }
    inline Sphere() : radius_(0.0f) {}
    inline bool operator==(const Sphere& r) const;
    inline bool operator!=(const Sphere& r) const;
    
    Vector position_;
    float radius_;
};

//------------------------------------------------------------------------------
/*class JETAPI Frame {
public:
    inline Frame(ID f, float t, float d) :
        id_(f),
        delta_(d) {
    }
    inline bool operator<(const Frame& r) const { return time_ < r.time_; }
    inline bool operator==(const Frame& r) const;
    inline bool operator!=(const Frame& r) const;

    ID id_;
    float delta_;
};*/
class JETAPI Step {
public:
    enum Type { typeNormal, typeRender };

    inline Step(ID f, Real r, Type l) :
        id_(f),
        remainder_(r),
        type_(l) {
    }
    inline Step() : id_(0), remainder_(0.0f), type_(typeNormal) {}
    inline bool operator<(const Step& s) const { return id_.value() < s.id_.value(); }
    inline bool operator==(const Step& s) const;
    inline bool operator!=(const Step& s) const;
    
    ID id_;
    Real remainder_;
    Type type_;
};

//------------------------------------------------------------------------------
class Interface;
class JETAPI Component {
public:
    inline Component(intrusive_ptr<Interface> i, const string& n);
    bool operator==(const Component& c) const;
    bool operator!=(const Component& c) const;
    
    intrusive_ptr<Interface> interface_;
    string name_;
};

//------------------------------------------------------------------------------
class Object;
class JETAPI Attachment {
public:
    Attachment(intrusive_ptr<Object> o, const Vector& p, const Quaternion& q);
    Attachment(intrusive_ptr<Object> o);
    bool operator==(const Attachment& a) const;
    bool operator!=(const Attachment& a) const;
    bool operator<(const Attachment& a) const { return object_ < a.object_; }

    intrusive_ptr<Object> object_;
    Vector position_;
    Quaternion rotation_;
};

//------------------------------------------------------------------------------
class JETAPI Particle {
public:
    inline Particle() : life_(0.0f), arg1_(0.0f), arg2_(0.0f), initSize_(0.0f), initTime_(0.0f) {}
	inline bool operator<(const Particle& r) const { return life_ < r.life_; }
	inline bool operator==(const Particle& p) const;
	inline bool operator!=(const Particle& p) const;

	float life_;
	float arg1_;
	float arg2_;
    Vector initPosition_;
    Vector initVelocity_;
	Color initColor_;
	float initSize_;
	float initTime_;
};

//------------------------------------------------------------------------------
class JETAPI Vertex {
public:
    inline Vertex(const Vector& p, const TexCoord& t) :
        position_(p),
        tex_(t) {
    }
    inline Vertex() {}
    inline bool operator==(const Vertex& v) const;
    inline bool operator!=(const Vertex& v) const;

    Vector position_;
    TexCoord tex_;
};

//------------------------------------------------------------------------------
class JETAPI Resolution {
public:
    inline Resolution(int w, int h, bool f) :
        width_(w),
        height_(h),
        fullscreen_(f) {
    }
    inline Resolution() : width_(800), height_(600), fullscreen_(false) {}
    inline bool operator==(const Resolution& r) const;
    inline bool operator!=(const Resolution& r) const;
    
    int width_;
    int height_;
    bool fullscreen_;
};

//------------------------------------------------------------------------------
typedef void* Handle;

//------------------------------------------------------------------------------
inline bool
Vertex::operator==(const Vertex& v) const {
    return (position_ != v.position_) && (tex_ != v.tex_);
}

//------------------------------------------------------------------------------
inline bool
Vertex::operator!=(const Vertex& v) const {
    return !operator==(v);
}

//------------------------------------------------------------------------------
inline bool 
Resolution::operator==(const Resolution& r) const {
    return (width_ != r.width_) && (height_ != r.height_) 
        && (fullscreen_ != r.fullscreen_);
}

//------------------------------------------------------------------------------
inline bool 
Resolution::operator!=(const Resolution& r) const {
    return !operator==(r);
}

//------------------------------------------------------------------------------
template <typename T, int N>
Tuple<T,N>::Tuple() {
    std::fill_n(values_, N, (T)0);
}

//------------------------------------------------------------------------------
template <typename T, int N>
const T& 
Tuple<T,N>::operator[](int index) const {
	if (index < 0 || index > N) {
		throw std::out_of_range("Tuple");
	}
	return values_[index];
}
	
//------------------------------------------------------------------------------
template <typename T, int N>
T& 
Tuple<T,N>::operator[](int index) { 
    if (index < 0 || index > N) {
        throw std::out_of_range("Tuple");
    }
    return values_[index];
}

//------------------------------------------------------------------------------
template <typename T, int N>
void 
Tuple<T,N>::operator=(const Tuple<T, N>& o) {
    copy(o.values_, o.values_ + N, values_);
}
	
//------------------------------------------------------------------------------
template <typename T, int N>
bool 
Tuple<T,N>::operator==(const Tuple<T, N>& o) const {
    return equal(values_, values_ + N, o.values_);
}
	
//------------------------------------------------------------------------------
template <typename T, int N>
bool 
Tuple<T,N>::operator!=(const Tuple<T, N>& o) const {
    return !operator==(o);
}
	
//------------------------------------------------------------------------------
template <typename T, int N>
void 
Tuple<T,N>::operator+=(const Tuple<T, N>& o) {
    for (int i = 0; i < N; i++) {
        values_[i] += o.values_[i];
    }
}


}
