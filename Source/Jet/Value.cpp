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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOVT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BVT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AVTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OVT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */  
#pragma once

#include <Jet/Value.hpp>
#include <boost/lexical_cast.hpp>

using namespace Jet;
using namespace std;
using namespace boost;

const Value Value::NIL;

Value::Value() {
    type_ = VT_NIL;
}   

Value::Value(const Value& value) {
    type_ = value.type_;
    switch (type_) {
        case VT_STRING: string_ = new string(*value.string_); break;
        case VT_NUMBER: number_ = new number_t(*value.number_); break;
        case VT_COLOR: color_ = new Color(*value.color_); break;
        case VT_VECTOR: vector_ = new Vector(*value.vector_); break;
        case VT_QUATERNION: quaternion_ = new Quaternion(*value.quaternion_); break;
        case VT_RANGE: range_ = new Range(*value.range_); break;
    }
}

Value::Value(const string& string) {
    type_ = VT_STRING;
    string_ = new std::string(string);
}

Value::Value(number_t number) {
    type_ = VT_NUMBER;
    number_ = new number_t(number);
}

Value::Value(const Color& color) {
    type_ = VT_COLOR;
    color_ = new Color(color);
}

Value::Value(const Vector& vector) {
    type_ = VT_VECTOR;
    vector_ = new Vector(vector);
}

Value::Value(const Quaternion& quaternion) {
    type_ = VT_QUATERNION;
    quaternion_ = new Quaternion(quaternion);
}

Value::Value(const Range& range) {
    type_ = VT_RANGE;
    range_ = new Range(range);
}

Value::Value(const char* str) {
    type_ = VT_STRING;
    string_ = new string(str);
}

Value::~Value() {
    clear();
}

Value& Value::operator=(const Value& value) {
    clear();
    type_ = value.type_;
    switch (type_) {
        case VT_STRING: string_ = new string(*value.string_); break;
        case VT_NUMBER: number_ = new number_t(*value.number_); break;
        case VT_COLOR: color_ = new Color(*value.color_); break;
        case VT_VECTOR: vector_ = new Vector(*value.vector_); break;
        case VT_QUATERNION: quaternion_ = new Quaternion(*value.quaternion_); break;
        case VT_RANGE: range_ = new Range(*value.range_); break;
    }
    return *this;
}

Value& Value::operator=(const std::string& string) {
    clear();
    type_ = VT_STRING;
    string_ = new std::string(string);
    return *this;
}

Value& Value::operator=(number_t number) {
    clear();
    type_ = VT_NUMBER;
    number_ = new number_t(number);
    return *this;
}

Value& Value::operator=(const Color& color) {
    clear();
    type_ = VT_COLOR;
    color_ = new Color(color);
    return *this;
}

Value& Value::operator=(const Vector& vector) {
    clear();
    type_ = VT_VECTOR;
    vector_ = new Vector(vector);
    return *this;
}

Value& Value::operator=(const Quaternion& quaternion) {
    clear();
    type_ = VT_QUATERNION;
    quaternion_ = new Quaternion(quaternion);
    return *this;
}

Value& Value::operator=(const Range& range) {
    clear();
    type_ = VT_RANGE;
    range_ = new Range(range);
    return *this;
}

Value::operator std::string() const {
    switch(type_) {
        case VT_STRING: return *string_;
        case VT_NUMBER: return lexical_cast<string>(*number_);
        case VT_COLOR: return lexical_cast<string>(*color_);
        case VT_VECTOR: return lexical_cast<string>(*vector_);
        case VT_QUATERNION: return lexical_cast<string>(*quaternion_);
        case VT_RANGE: return lexical_cast<string>(*range_);
        case VT_NIL: return "nil";
        default: return "nil";
    }
}

Value::operator number_t() const {
    switch (type_) {
        case VT_STRING: *const_cast<Value*>(this) = lexical_cast<number_t>(*string_); // fallthrough
        case VT_NUMBER: return *number_;
        default: throw runtime_error("Invalid type conversion");
    }   
}

Value::operator Color() const {
    switch (type_) {
        case VT_STRING: *const_cast<Value*>(this) = lexical_cast<Color>(*string_); // fallthrough
        case VT_COLOR: return *color_;
        default: throw runtime_error("Invalid type conversion");
    }
}

Value::operator Vector() const {
    switch (type_) {
        case VT_STRING: *const_cast<Value*>(this) = lexical_cast<Vector>(*string_); // fallthrough
        case VT_VECTOR: return *vector_;
        default: throw runtime_error("Invalid type conversion");
    }
}

Value::operator Quaternion() const {
    switch (type_) {
        case VT_STRING: *const_cast<Value*>(this) = lexical_cast<Quaternion>(*string_); // fallthrough
        case VT_QUATERNION: return *quaternion_;
        default: throw runtime_error("Invalid type conversion");
    }
}

Value::operator Range() const {
    switch (type_) {
        case VT_STRING: *const_cast<Value*>(this) = lexical_cast<Range>(*string_); // fallthrough
        case VT_RANGE: return *range_;
        default: throw runtime_error("Invalid type conversion");
    }
}

void Value::clear() {
    switch (type_) {
        case VT_STRING: delete string_; break;
        case VT_NUMBER: delete number_; break;
        case VT_COLOR: delete color_; break;
        case VT_VECTOR: delete vector_; break;
        case VT_QUATERNION: delete quaternion_; break;
        case VT_RANGE: delete range_; break;
    }
    type_ = VT_NIL;
}

