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

#include <Jet/Types.hpp>
#include <Jet/Interface.hpp>
#include <Jet/Publisher.hpp>

namespace Jet {
using namespace std;
using namespace std::tr1;

class Root;

class JETAPI Options : public Interface {
public:
    class Observer;
    friend class Root;
    typedef intrusive_ptr<Options> Ptr;
    typedef RangedOrdinal<float, 0, 1> Volume;
    
    enum Antialiasing { antialiasDisabled, antialiasEnabled };
    enum Quality { qualityLow, qualityHigh };
    enum Bloom { bloomDisabled, bloomEnabled };

    // Attributes
    inline Volume               masterVolume() const { return masterVolume_; }
    void                        masterVolume(Volume v);
    inline float                physicsDelta() const { return physicsDelta_; }
    void                        physicsDelta(float t);
    inline float                renderDelta() const { return renderDelta_; }
    void                        renderDelta(float t);
    inline const VideoMode&     videoMode() const { return videoMode_; }
    void                        videoMode(const VideoMode& r);

    // Utility
    inline Publisher<Observer>& publisher() const { return publisher_; }

private:
    Options();

    mutable Publisher<Observer> publisher_;
    Volume masterVolume_;
    float physicsDelta_;
    float renderDelta_;
    VideoMode videoMode_;
    
};

class Options::Observer : public virtual Interface {
public:
    typedef intrusive_ptr<Options::Observer> Ptr;
    
    virtual void onMasterVolume() {}
    virtual void onPhysicsDelta() {}
    virtual void onRenderDelta() {}
    virtual void onVideoMode() {}
};

}
