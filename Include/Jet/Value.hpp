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
#include <Jet/Color.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Range.hpp>
#include <iostream>

JETAPI std::ostream& operator<<(std::ostream& out, const Jet::Value& value);
JETAPI std::istream& operator>>(std::istream& in, Jet::Value& value);

namespace Jet {

//! Represents any type of value.
//! @class Value
//! @brief Represents any type of value
class JETAPI Value {
public:
    Value();
    Value(const Value& value);
    Value(const std::string& string);
    Value(number_t number);
    Value(const Color& color);
    Value(const Vector& vector);
    Value(const Quaternion& quaternion);
    Value(const Range& range);
    Value(const char* string);
    ~Value();

    Value& operator=(const Value& other);
    Value& operator=(const std::string& string);
    Value& operator=(number_t number);
    Value& operator=(const Color& color);
    Value& operator=(const Vector& vector);
    Value& operator=(const Quaternion& quaternion);
    Value& operator=(const Range& range);
    
    operator std::string() const;
    operator number_t() const;
    operator real_t() const;
    operator int32_t() const;
    operator uint32_t() const;
    operator Color() const;
    operator Vector() const;
    operator Quaternion() const;
    operator Range() const;
    operator bool() const;
    
    
    inline ValueType type() const {
        return type_;
    }
    

    static const Value NIL;
    
private:
    void clear();

    ValueType type_;
    union {
        std::string* string_;
        number_t* number_;
        Color* color_;
        Vector* vector_;
        Quaternion* quaternion_;
        Range* range_;
    };
};

typedef std::pair<const std::string, Value> ValueEntry;

}

