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
#include <boost/any.hpp>

namespace Jet {

//! This is the main engine class.  This object is the parent of all other
//! objects in the game engine.
//! @class Engine
//! @brief Main engine class.
class Engine : public Object {
public:
	
    //! Returns the root scene node.  This node can be used to add 
    //! other objects to the scene.
    virtual Node* root() const=0;
	
	//! Returns the current module.
	virtual Module* module() const=0;
        
    //! Returns true if the engine is running
    virtual bool running() const=0;
	
	//! Returns the simulation speed of the engine
	virtual real_t simulation_speed() const=0;
    
    //! Returns an engine option.  This method will throw an exception if
    //! the option could not be found.
    //! @param name the name of the option
    virtual const boost::any& option(const std::string& name) const=0;

    //! Returns the material with the given name.
    //! @param name the name of the material
    virtual Material* material(const std::string& type)=0;

    //! Returns the mesh with the given name.
    //! @param name the name of the mesh.
    virtual Mesh* mesh(const std::string& name)=0;

    //! Returns the given texture descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    virtual Texture* texture(const std::string& name)=0;
    
    //! Returns the given shader descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    virtual Shader* shader(const std::string& name)=0;
    
    //! Returns the full path to the file using the given resource name.
    //! @param name the name of the resource
    virtual std::string resource_path(const std::string& name) const=0;
    
    //! Adds a folder to the search path for loading resources.  Resources
    //! will be loaded automatically.
    //! @param folder the folder to add
    virtual void search_folder(const std::string& path)=0;
	
	//! Sets the current module.  The previous module will be deactivated.
	//! @param module the module
	virtual void module(Module* module)=0;
    
    //! Sets whether or not the engine is running.
    //! @param running false to stop the engine
    virtual void running(bool running)=0;
	
	//! Sets the speed factor for the physical simulation.
	virtual void simulation_speed(real_t speed)=0;
	
    //! Adds a listener, which listens for engine events.
    //! @param listener the engine listener.
    virtual void listener(EngineListener* listener)=0;
      
    //! Sets an engine option.  For a list of possible options, see the
    //! documentation.  Note that individual plugins may have their own
    //! set of options; see the documentation for the plugin to find the
    //! available options.
    //! @param name the name of the option
    //! @param value the value of the option
    virtual void option(const std::string& name, const boost::any& value)=0;
    
    //! Runs the engine through one complete loop.  Note that the engine may
    //! or may not actually do anything on a given loop, depending on the
    //! elapsed time.
    virtual void tick()=0;
	
	//! Creates a new engine.
	static Engine* create();
};

//! Listens for engine events.  Examples include on_pre_render (called before
//! rendering), on_post_render (called after rendering), on_render (called
//! during rendering), and on_update (called with each tick of the physics
//! engine.
//! @class EngineListener
//! @brief Interface for handling engine events.
class EngineListener : public Object {
public:
	//! Called when the engine is initialized
	virtual void on_init()=0;
	
    //! Called for each physics update.
    virtual void on_update()=0;
	
	//! Called after a batch of physics updates has finished, but before
	//! the frame is rendered.
	virtual void on_post_update()=0;
    
    //! Called when the frame is rendered.
    virtual void on_render()=0;
};

}


/*
http://!!warningdonottypetoohard--@xoxoalsodonotleavetomorrrowww.stayhere.com
by jilli
*/
