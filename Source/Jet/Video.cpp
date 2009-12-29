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

#include <Jet/Video.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
void                        
Video::quality(Quality q) {
    if (q != quality_) {
        quality_ = q;
        publisher_.notify(&Observer::onQuality);
    }
}

//------------------------------------------------------------------------------
void
Video::antialiasing(Antialiasing a) {
    if (a != antialiasing_) {
        antialiasing_ = a;
        publisher_.notify(&Observer::onAntialiasing);
    }
}

//------------------------------------------------------------------------------
void
Video::bloom(Bloom b) {
    if (b != bloom_) {
        bloom_ = b;
        publisher_.notify(&Observer::onBloom);
    }
}

//------------------------------------------------------------------------------
void
Video::state(State s) {
    if (s != state_) {
        state_ = s;
        publisher_.notify(&Observer::onState);
    }
}
