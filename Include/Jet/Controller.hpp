

/*
 * Copyright (c) 2010 Matt Fichman
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
#include <Jet/Params.hpp>

namespace Jet {

//! Controller that can control the behavior of a node.
//! @class Controller
//! @brief Controller that can be attached to a scene node.
class JETAPI Controller : public Object {
public:
    
    //! Destructor
    virtual ~Controller() {}

    //! Returns the parent node.
    inline Node* parent() const {
        return parent_;
    }

    //! Called when an event occurs regarding the current node.
    //! @param name the name of the event.
    //! @param params the parameters for the event.
    virtual void on_event(const std::string& name, const Params& params=Params())=0;
    
private:
    //! Clones this controller.
    virtual Controller* clone() const=0;

#pragma warning(disable:4251)
    Node* parent_;
#pragma warning(default:4251)

    friend class Node;
};

}
