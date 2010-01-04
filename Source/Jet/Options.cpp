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

#include <Jet/Options.hpp>

using namespace Jet;

//------------------------------------------------------------------------------
Options::Options() :
    masterVolume_(0.5f),
    quality_(qualityHigh),
    antialiasing_(antialiasEnabled),
    bloom_(bloomEnabled),
    physicsDelta_(0.01f),
    renderDelta_(0.02f) {
    
}

//------------------------------------------------------------------------------
void
Options::masterVolume(Volume v) {
    if (v != masterVolume_) {
        masterVolume_ = v;
        publisher_.notify(&Observer::onMasterVolume);
    }
}

//------------------------------------------------------------------------------
void                        
Options::quality(Quality q) {
    if (q != quality_) {
        quality_ = q;
        publisher_.notify(&Observer::onQuality);
    }
}

//------------------------------------------------------------------------------
void
Options::antialiasing(Antialiasing a) {
    if (a != antialiasing_) {
        antialiasing_ = a;
        publisher_.notify(&Observer::onAntialiasing);
    }
}

//------------------------------------------------------------------------------
void
Options::bloom(Bloom b) {
    if (b != bloom_) {
        bloom_ = b;
        publisher_.notify(&Observer::onBloom);
    }
}
