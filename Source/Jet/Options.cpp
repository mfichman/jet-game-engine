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
    physicsDelta_(0.01f),
    renderDelta_(0.02f) {
    
    videoMode_.width_ = 800;
    videoMode_.height_ = 600;
    videoMode_.windowMode_ = VideoMode::modeWindowed;
    videoMode_.antialiasing_ = VideoMode::antialiasEnabled;
    videoMode_.quality_ = VideoMode::qualityHigh;
    videoMode_.bloom_ = VideoMode::bloomEnabled;
    videoMode_.shaderMode_ = VideoMode::shadersEnabled;
    
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
Options::videoMode(const VideoMode& r) {
    if (r != videoMode_) {
        videoMode_ = r;
        publisher_.notify(&Observer::onVideoMode);
    }
}
