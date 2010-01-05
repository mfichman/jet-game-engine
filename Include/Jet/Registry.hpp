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

#include <Jet/Class.hpp>
#include <Jet/Interface.hpp>
#include <map>
#include <iostream>

namespace Jet {
using namespace std;
using namespace std::tr1;
using namespace std::tr1::placeholders;

class Root;

class JETAPI Registry {

public:
    typedef std::tr1::shared_ptr<Registry> Ptr;
    friend class Root;
    
    template <typename T>
    Class::Ptr type() {
        const type_info* info = &typeid(T);
        map<const type_info*, Class::Ptr>::iterator i = type_.find(info);
        if (i == type_.end()) {
            i = type_.insert(make_pair(info, Class::Ptr(new Class()))).first;
        }
        return i->second;
    }
    
    Class::Ptr type(Interface::Ptr iface) {
        const type_info* info = &typeid(*iface);
        map<const type_info*, Class::Ptr>::iterator i = type_.find(info);
        if (i == type_.end()) {
            i = type_.insert(make_pair(info, Class::Ptr(new Class()))).first;
        }
        return i->second;
    }
   
private:
    Registry();
    map<const type_info*, Class::Ptr> type_;
};

}
