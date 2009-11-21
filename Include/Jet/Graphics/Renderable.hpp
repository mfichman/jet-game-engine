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

namespace Jet { namespace Graphics {
using namespace std;
using namespace std::tr1;

class Renderable {
public:   
    typedef shared_ptr<Renderable> Ptr;
    typedef RangedOrdinal<int, 0, 256> RenderPriority;
    enum Visibility { visible, invisible };

    RenderPriority  renderPriority() const { return renderPriority_; }
    void            renderPriority(RenderPriority p) { renderPriority_ = p; }
    Visibility      visibility() const { return visibility_; }
    void            visibility(Visibility v) { visibility_ = v; }
    
protected:
    Renderable() : renderPriority_(0) {}
    RenderPriority  renderPriority_;
    Visibility      visibility_;    
};

}}
