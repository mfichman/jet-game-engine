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
#include <Jet/Anchor.hpp>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;
class Root;

class Speaker : public Interface {
public:
    class Observer;
    friend class Root;
    typedef RangedOrdinal<float, 0, 1> Volume;
    typedef intrusive_ptr<Speaker> Ptr;

    // Attributes
    inline string           clip() const { return clip_; }
    void                    clip(const string& s);
    inline Volume           volume() const { return volume_; }
    void                    volume(Volume v);
    inline int              channels() const { return channels_; }
    void                    channelsInc();
    void                    channelsDec();

    // Components
    inline Object::Ptr      object() const { return object_; }
    inline Anchor::Ptr      anchor() const { return anchor_; }

    // Utility
    inline Publisher<Observer>& publisher() const { return publisher_; }

protected:
    Speaker() : object_(new Object), anchor_(new Anchor) {}

    mutable Publisher<Observer> publisher_;
    Object::Ptr object_;
    Anchor::Ptr anchor_;
    int channels_;
    string clip_;     
    Volume volume_;
};

class Speaker::Observer : public Interface {
public:
    typedef intrusive_ptr<Speaker::Observer> Ptr;

    virtual void onClip();
    virtual void onVolume();
    virtual void onChannelsInc();
    virtual void onChannelsDec();
};

}
