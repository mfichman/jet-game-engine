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

#include <Jet/Types.hpp>
#include <Jet/Graphics/Overlay.hpp>

namespace Jet { namespace Graphics {
using namespace std;
using namespace std::tr1;

class Engine;

class TextBox : public Overlay, public Interface {
public:
    friend class Engine;
    typedef std::tr1::shared_ptr<TextBox> Ptr;
    

    std::string     fontFamily() const { return fontFamily_; }
    void            fontFamily(const std::string& s) { fontFamily_ = s; }
    FontPoint       fontSize() const { return fontSize_; }
    void            fontSize(FontPoint p) { fontSize_ = p; }
    Color           fontColor() const { return fontColor_; }
    void            fontColor(const Color& c) { fontColor_ = c; }
    string			text() const { return text_; } 
    void            text(const std::string& t) { text_ = t; }
    //void            charBuffer(CharBuffer::Ptr k) { charBuffer_ = k; }
    Dimension       size() const { return size_; }
    void            size(const Dimension& s) { size_ = s; }
 
protected:
    TextBox() {}
    std::string     fontFamily_;
    FontPoint       fontSize_;
    Color           fontColor_;
    string			text_;
    //CharBuffer      charBuffer_;
    Dimension       size_;
};

}}

