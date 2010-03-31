

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

    //! Called when an event occurs regarding the current node.
    //! @param node the node that generated the event
    //! @param name the name of the event.
    //! @param params the parameters for the event.
    virtual void on_event(Node* node, const std::string& name, const Params& params=Params())=0;
    
    //! Called during each physics update.  Note: this function is called
    //! once per game tick, which is a regular interval fixed by default to
    //! 60 Hz.
    //! @param node the node that generated the event
    virtual void on_update(Node* node)=0;
    
    //! Called during each frame.  Note: this function is NOT called once per
    //! game tick.  It is called whenever the frame is rendered, and may not
    //! be called at 60 Hz.  Place all time-dependent code in on_update
    //! instead.  Also note that if the object is culled from the scene, this
    //! method will not be called.
    //! @param node the node that generated the event
    virtual void on_render(Node* node)=0;
    
private:
    //! Clones this controller.
    virtual Controller* clone() const=0;
    
    friend class Node;
};

}
