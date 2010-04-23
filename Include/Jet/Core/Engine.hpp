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

#include <Jet/Core/Types.hpp>
#include <Jet/Iterator.hpp>
#include <Jet/Types.hpp>
#include <Jet/Engine.hpp>
#include <Jet/Camera.hpp>
#include <Jet/Module.hpp>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <boost/any.hpp>
#ifndef WINDOWS
#include <sys/time.h>
#endif

namespace Jet { namespace Core {

//! This is the main engine class.  This object is the parent of all other
//! objects in the game engine.
//! @class Engine
//! @brief Main engine class.
class Engine : public Jet::Engine {
public:    
    //! Constructor
    Engine();

    //! Destructor
    virtual ~Engine();

    //! Returns the root scene node.  This node can be used to add 
    //! other objects to the scene.
    inline Jet::Node* root() const { 
        return root_.get(); 
    }
        
	//! Returns the active camera.
	inline Jet::Camera* camera() const {
		return camera_.get();
	}
	
	//! Returns the physics system.
	inline PhysicsSystem* physics_system() const {
		return physics_system_.get();
	}
	
	//! Returns the render system.
	inline RenderSystem* render_system() const {
		return render_system_.get();
	}
	
	//! Returns the script system.
	inline ScriptSystem* script_system() const {
		return script_system_.get();
	}
	
	//! Returns the current module.
	inline Module* module() const {
		return module_.get();
	}
	
    //! Returns true if the engine is running
    inline bool running() const {
        return running_;
	}
	
	//! Returns the fixed timestep
	inline real_t timestep() const {
		return 1.0f/60.0f;
	}
	
	//! The delta since the last physics step
	inline real_t delta() const {
		return delta_;
	}
	
	//! Returns the simulation speed.
	inline real_t simulation_speed() const {
		return simulation_speed_;
	}
    
    //! Returns an engine option.  This method will return nil if the option
    //! cannot bet found.
    //! @param name the name of the option
    inline const boost::any& option(const std::string& name) {
        return option_[name];
    }
    
    //! Returns an engine option.  This method will throw an exception if
    //! the option could not be found.
    //! @param name the name of the option
    inline const boost::any& option(const std::string& name) const {
        std::map<std::string, boost::any>::const_iterator i = option_.find(name);
        if (i != option_.end()) {
            return i->second;
        } else {
			throw std::runtime_error("Option not found: " + name);
        }
    }

    //! Returns the full path to the file using the given resource name.
    //! @param name the name of the resource
    std::string resource_path(const std::string& name) const;

    //! Returns the material with the given name.
    //! @param name the name of the material
    Jet::Material* material(const std::string& type);

    //! Returns the mesh with the given name.
    //! @param name the name of the mesh.
    Jet::Mesh* mesh(const std::string& name);

    //! Returns the given texture descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    Jet::Texture* texture(const std::string& name);
    
    //! Returns the given shader descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    Jet::Shader* shader(const std::string& name);


    //! Adds a listener, which listens for engine events.
    //! @param listener the engine listener.
    inline void listener(EngineListener* listener) {
        listener_.push_back(listener);
    }
    
    //! Adds a folder to the search path for loading resources.  Resources
    //! will be loaded automatically.
    //! @param folder the folder to add
    inline void search_folder(const std::string& path) {
        search_folder_.insert(resolve_path(path));
    }
	
	//! Sets the current module.
	//! @param module the module
	inline void module(Module* module) {
		if (module_) {
			module_->on_destroy();
			module_.reset();
		}
		module_ = module;
		if (module_) {
			module_->on_init();
		}
	}
    
    //! Sets whether or not the engine is running.
    //! @param running false to stop the engine
    inline void running(bool running) {
        running_ = running;
    }
	
	//! Sets the simulation speed)
	inline void simulation_speed(real_t speed) {
		simulation_speed_ = speed;
	}
	
	//! Sets the active camera.
	inline void camera(Jet::Camera* camera) {
		camera_ = camera;
	}
	
    //! Sets an engine option.  For a list of possible options, see the
    //! documentation.  Note that individual plugins may have their own
    //! set of options; see the documentation for the plugin to find the
    //! available options.
    //! @param name the name of the option
    //! @param value the value of the option
    inline void option(const std::string& name, const boost::any& value) {
        option_[name] = value;
    }
	
	//! Returns an iterator over all the search folders.
	inline Iterator<const std::string> search_folders() const {
		return Iterator<const std::string>(search_folder_.begin(), search_folder_.end());
	}
	
	//! Returns an iterator to the meshes
	inline Iterator<std::pair<const std::string, Jet::MeshPtr> > meshes() {
		return Iterator<std::pair<const std::string, Jet::MeshPtr> >(mesh_.begin(), mesh_.end());
	}
	
	//! Returns an iterator to the meshes
	inline Iterator<std::pair<const std::string, Jet::TexturePtr> > textures() {
		return Iterator<std::pair<const std::string, Jet::TexturePtr> >(texture_.begin(), texture_.end());
	}
	
	//! Returns an iterator to the meshes
	inline Iterator<std::pair<const std::string, Jet::ShaderPtr> > shaders() {
		return Iterator<std::pair<const std::string, Jet::ShaderPtr> >(shader_.begin(), shader_.end());
	}
    
    //! Runs the engine through one complete loop.  Note that the engine may
    //! or may not actually do anything on a given loop, depending on the
    //! elapsed time.
    void tick();	
	
	template <typename T>
	inline T option(const std::string& name) const {
		try {
			return boost::any_cast<T>(option(name));
		} catch (boost::bad_any_cast) {
			std::string type;
			if (typeid(T) == typeid(std::string)) {
				type = "string";
			} else {
				type = typeid(T).name();
			}
			throw std::runtime_error("Option '" + name + "' is invalid: expected type '" + type + "'");
		}
	}

private:
	friend class PhysicsSystem;
    std::string resolve_path(const std::string& path);
    void update_delta();
	void init_systems();

	
	std::map<std::string, boost::any> option_;

    Jet::NodePtr root_;
	Jet::CameraPtr camera_;
	Jet::ModulePtr module_;
	
	PhysicsSystemPtr physics_system_;
	RenderSystemPtr render_system_;
	ScriptSystemPtr script_system_;
    
    std::map<std::string, Jet::MaterialPtr> material_;
    std::map<std::string, Jet::MeshPtr> mesh_;
    std::map<std::string, Jet::TexturePtr> texture_;
	std::map<std::string, Jet::ShaderPtr> shader_;
    
    std::list<EngineListenerPtr> listener_;
    std::set<std::string> search_folder_;
    
    
    bool running_;
	bool initialized_;
    real_t accumulator_;
    real_t delta_;
	real_t simulation_speed_;
#ifdef WINDOWS
    float secs_per_count_;
    int64_t prev_time_;
#else
	timeval prev_time_;
#endif
    
};

}}


/*
http://!!warningdonottypetoohard--@xoxoalsodonotleavetomorrrowww.stayhere.com
by jilli
*/
