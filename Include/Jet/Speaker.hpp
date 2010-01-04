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
#include <Jet/Object.hpp>
#include <Jet/Publisher.hpp>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;
class Root;

class JETAPI Speaker : public Object {
public:
    class Observer;
    friend class Root;
    typedef RangedOrdinal<float, 0, 1> Volume;
    typedef intrusive_ptr<Speaker> Ptr;
    enum State { statePlay, stateLoop, stateStop };

    // Attributes
    inline string               clip() const { return clip_; }
    void                        clip(const string& s);
    inline Volume               volume() const { return volume_; }
    void                        volume(Volume v);
    inline State                state() const { return state_; }
    void                        state(State s);
    inline void                 operator()(Object::Functor& f) { f(this); }

    // Utility
    inline Publisher<Observer>& publisher() const { return publisher_; }

protected:
    Speaker() {}

    mutable Publisher<Observer> publisher_;
    string clip_;     
    Volume volume_;
    State state_;
};

class Speaker::Observer : public Interface {
public:
    typedef intrusive_ptr<Speaker::Observer> Ptr;

    virtual void onClip()=0;
    virtual void onVolume()=0;
    virtual void onState()=0;
};

}
