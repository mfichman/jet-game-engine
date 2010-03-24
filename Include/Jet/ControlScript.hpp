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

#include <Jet/Jet.hpp>
#include <Jet/Object.hpp>
#include <Jet/Vector.hpp>
#include <string>

namespace Jet {

//! Used to embed logic in a scene node.
//! @class ControlScript
//! @brief Embeds logic in a scene node
class ControlScript : public Object {
public:

    //! Called when the control script is attached to the scene component  
    virtual void on_attach(const AttachmentEvent& evt)=0;
    
    //! Called when the rigid body belogning to the parent scene node
    //! collides with another rigid body
    virtual void on_collision(const CollisionEvent& evt)=0;

    //! Called during a game update
    virtual void on_tick(const TickEvent& evt)=0;

protected:
    //! Clones this control script
    virtual ControlScript* clone()=0;
};

struct AttachmentEvent {
    //! The node the controlscript is attached to.
    SceneNode* node; 
};

struct CollisionEvent {
    //! The scene node involved in the collision
    SceneNode* node;

    //! The node that this node collided with.
    SceneNode* other;

    //! The position at which the two nodes are touching.
    Vector contact_position;

    //! The normal at the contact point.
    Vector contact_normal;
};

struct TickEvent {
    //! Scene node that is the source of this event
    SceneNode* node;

    //! The time since the last tick.  This is always fixed.
    real_t delta;

    //! The tick number.  Starts at zero and increments with teach tick.
    uint32_t tick;
};

}
