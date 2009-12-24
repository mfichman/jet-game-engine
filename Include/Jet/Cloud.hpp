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
#include <Jet/Iterator.hpp>
#include <Jet/Collidable.hpp>
#include <vector>
#include <algorithm>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;
class Root;

class Cloud : public Interface {
public:
    friend class Root;
    typedef intrusive_ptr<Cloud> Ptr;

    inline Iterator<vector<Particle> > particleItr() { return Iterator<vector<Particle> >(particle_); }
    Particle&                   topParticle() { return particle_.front(); }
    void                        topParticleDel() { pop_heap(particle_.begin(), particle_.end()); }
    void                        particleNew(Particle& o);
    inline const string&        particleMethod() { return particleMethod_; }
    void                        particleMethod(const string& s);
    
    // Components
    inline Object::Ptr          object() const { return object_; }
    inline Anchor::Ptr          anchor() const { return anchor_; }
    inline Collidable::Ptr      collidable() const { return collidable_; }

protected:
    Cloud() : object_(new Object), anchor_(new Anchor), collidable_(new Collidable) {}

    Object::Ptr object_;
    Anchor::Ptr anchor_;
    Collidable::Ptr collidable_;
    string particleMethod_;
    vector<Particle> particle_;
};

}
