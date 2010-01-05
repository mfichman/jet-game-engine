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
#include <string>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;

class JETAPI Resource : public Interface {
public:
    class Observer;
    typedef intrusive_ptr<Resource> Ptr;
    enum Status { statusUnloading, statusUnloaded, statusLoading, statusLoaded };
    
    // Attributes
    inline const string&    name() const { return name_; }
    inline Status           loadStatus() const { return loadStatus_; }
    void                    loadStatus(Status s); 
    inline Handle           handle() const { return handle_; }
    void                    handle(Handle h);  
    
    // Utility
    inline Publisher<Observer>& publisher() const { return publisher_; };
    
protected:
    Resource(const string& name);

    mutable Publisher<Observer> publisher_;
    string name_;
    Status loadStatus_;
    Handle handle_;
};

class Resource::Observer : public virtual Interface {
public:
    typedef intrusive_ptr<Resource::Observer> Ptr;
    
    virtual void onLoadStatus() {}
};

}
