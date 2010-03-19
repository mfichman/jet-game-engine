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

#include <Jet/Jet.hpp>
#include <Jet/Object.hpp>

namespace Jet {

//! This is the main engine class.  This object is the parent of all other
//! objects in the game engine.
//! @class Engine
//! @brief Main engine class.
class Engine : public Object {
public:
    
    //! Creates a new engine.
    Engine();

    //! Returns the root scene node.  This node can be used to add 
    //! other objects to the scene.
    SceneNode* root() { return root; }

    //! Adds an audio module to the engine.
    //! @param module the audio module
    void audio(AudioModule* module);

    //! Adds a graphics module to the engine.
    //! @param module the graphics module
    void graphics(GraphicsModule* module);

    //! Adds a physics module to the engine.
    //! @param module the physics module
    void physics(PhysicsModule* module);
    
    //! Adds a network module to the engine
    //! @param network module the network module
    void network(NetworkModule* module);

    //! Adds a logic module to the engine
    //! @param module the logic module
    void logic(LogicModule* module);

private:
    friend class SceneNode;

    SceneNode* root;
};

}
