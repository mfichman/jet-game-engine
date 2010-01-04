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
#include <Jet/Iterator.hpp>
#include <vector>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace boost;
class Root;

class JETAPI Entity : public Interface {
public:
    class Observer;
    friend class Root;
    typedef intrusive_ptr<Entity> Ptr;
    typedef Iterator<vector<Component> > ComponentItr;
    enum NetworkSync { disabled, enabled };
    
    // Attributes
    inline const string&    script() const { return script_; }
    void                    script(const string& s);
    inline const string&    data() const { return data_; }
    void                    data(const string& d);
    inline NetworkSync      networkSync() const { return networkSync_; }
    void                    networkSync(NetworkSync s);
    inline const string&    updateMethod() const { return updateMethod_; }
    void                    updateMethod(const string& s);
    inline double           time() const { return time_; }
    void                    time(double time);
    inline ComponentItr     componentItr() { return ComponentItr(component_); }
    void                    componentAdd(const Component& c);
    void                    componentDel(const Component& c);

    // Utility
    inline Publisher<Observer>& publisher() const { return publisher_; };

private:
    Entity() {}

    mutable Publisher<Observer> publisher_;
    string script_;
    string data_;
    NetworkSync networkSync_;
    string updateMethod_;
    double time_;
    vector<Component> component_;
};

class Entity::Observer : public Interface {
public:
    typedef intrusive_ptr<Entity::Observer> Ptr;
    
    virtual void onNetworkSync() {}
    virtual void onUpdateMethod() {}
    virtual void onTime() {}
    virtual void onComponentAdd(const Component&) {}
    virtual void onComponentDel(const Component&) {}
};

}
