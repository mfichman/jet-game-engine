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
using namespace boost;

class Collidable : public Interface {
public:
    class Observer;
    typedef intrusive_ptr<Collidable> Ptr;
    enum Solidity { phantom, solid };
    enum State { disabled, enabled };

    // Attributes
    const string&   collisionMethod() const { return collisionMethod_; }
    void            collisionMethod(const string& o);
    Solidity        solidity() const { return solidity_; }
    void            solidity(Solidity s);
    State           state() const { return state_; }
    void            state(State s);
    
    // Utility
    Publisher<Observer>& publisher() const { return publisher_; }

private:
    mutable Publisher<Observer> publisher_;
    string collisionMethod_;
    Solidity solidity_;
    State state_;   
};

class Collidable::Observer : public virtual Interface {
public:
    typedef intrusive_ptr<Collidable::Observer> Ptr;

    virtual void onCollisionMethod()=0;
    virtual void onSolidity()=0;
    virtual void onState()=0;
};

}
