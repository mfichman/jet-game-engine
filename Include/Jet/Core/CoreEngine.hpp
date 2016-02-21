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

#include <Jet/Core/CoreTypes.hpp>
#include <Jet/Types/Iterator.hpp>
#include <Jet/Resources/Texture.hpp>
#include <Jet/Resources/Cubemap.hpp>
#include <Jet/Resources/Geometry.hpp>
#include <Jet/Resources/Shader.hpp>
#include <Jet/Resources/Sound.hpp>
#include <Jet/Resources/Mesh.hpp>
#include <Jet/Resources/Font.hpp>
#include <Jet/Scene/Camera.hpp>
#include <Jet/Object.hpp>
#include <Jet/Engine.hpp>
#include <Jet/Physics.hpp>
#include <Jet/Network.hpp>
#include <Jet/Script.hpp>
#include <Jet/Audio.hpp>
#include <Jet/Input.hpp>
#include <Jet/Graphics.hpp>
#include <Jet/Module.hpp>
#include <Jet/Interface/Overlay.hpp>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <boost/any.hpp>

namespace Jet {

//! This is the main engine class.  This object is the parent of all other
//! objects in the game engine.
//! @class Engine
//! @brief Main engine class.
class CoreEngine : public Engine {
public:    
    //! Constructor
    CoreEngine();

    //! Destructor
    virtual ~CoreEngine();

    //! Returns the root scene node.  This node can be used to add 
    //! other objects to the scene.
    inline Node* root() const { 
        return root_.get(); 
    }
	
	//! Returns the root overlay.
	inline Overlay* screen() const {
		return screen_.get();
        
	}
	
	//! Returns the focused overlay.
	inline Overlay* focused_overlay() const {
		return focused_overlay_.get();
	}
	
	//! Returns the active camera.
	inline Camera* camera() const {
		return camera_.get();
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
	inline float timestep() const {
		return 1.0f/60.0f;
	}

	//! Frame accumulator
	inline float frame_accumulator() const {
		return frame_accumulator_;
	}
	
	//! The delta since the last render
	inline float frame_delta() const {
		return frame_delta_ * option<float>("simulation_speed");
	}
	
	//! Returns the frame time
	inline float frame_time() const {
		return frame_time_;
	}
    
    //! Returns the frame ID
    inline uint32_t frame_id() const {
        return frame_id_;
    }

	//! Returns the tick ID
	inline uint32_t tick_id() const {
		return tick_id_;
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
	
	//! Returns the font with the given name.
	//! @param name the name of the font
	Font* font(const std::string& name="");
	
	//! Returns the sound with the given name.
	//! @param name the name of the sound.
	Sound* sound(const std::string& name="");
	
    //! Returns the material with the given name.
    //! @param name the name of the material
    Material* material(const std::string& name="");

    //! Returns the mesh with the given name.
    //! @param name the name of the mesh.
    Mesh* mesh(const std::string& name="");
	
	//! Returns a mesh that is a copy of the given mesh.  The new mesh will
	//! share a read-only reference to the old mesh's vertex buffer, but will
	//! have its own index buffer.
    Mesh* mesh(Mesh* mesh);

    //! Returns the given texture descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    Texture* texture(const std::string& name="");
	
	//! Returns the given texture descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    Cubemap* cubemap(const std::string& name="");

    //! Returns the given shader descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    Shader* shader(const std::string& name="");
	
	//! Returns the geometry
	Geometry* geometry(const std::string& name="");
		
	//! Returns the network interface.
	Network* network() const;
	
	//! Returns the physics interface.
	Physics* physics() const;
	
	//! Returns the audio module
	Audio* audio() const;
	
	//! Returns script module
	Script* script() const;
	
	//! Returns renderer module
	Graphics* graphics() const;
	
	//! Returns the input system
	Input* input() const;
	
	//! Returns the network interface.
	inline void network(Network* network) {
		network_ = network;
	}
	
	//! Returns the physics interface.
	inline void physics(Physics* physics) {
		physics_ = physics;
	}
	
	//! Returns the audio module
	inline void audio(Audio* audio) {
		audio_ = audio;
	}
	
	//! Returns script module
	inline void script(Script* script) {
		script_ = script;
	}
		
	//! Returns renderer module
	inline void graphics(Graphics* graphics) {
		graphics_ = graphics;
	}
	
	//! Sets the input module
	inline void input(Input* input) {
		input_ = input;
	}

	//! Sets the tick id
	inline void tick_id(uint32_t tick) {
		tick_id_ = tick;
	}

	//! Increments the tick count
	inline void tick_id_inc() {
		frame_accumulator_ -= timestep();
		tick_id_++;
	}

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
	
	//! Sets the focused overlay.
	inline void focused_overlay(Overlay* overlay) {
		focused_overlay_ = overlay;
	}
	
	//! Sets the active camera.
	inline void camera(Camera* camera) {
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
	inline Iterator<std::pair<const std::string, MeshPtr> > meshes() {
		return Iterator<std::pair<const std::string, MeshPtr> >(mesh_.begin(), mesh_.end());
	}
	
	//! Returns an iterator to the meshes
	inline Iterator<std::pair<const std::string, TexturePtr> > textures() {
		return Iterator<std::pair<const std::string, TexturePtr> >(texture_.begin(), texture_.end());
	}
	
	//! Returns an iterator to the meshes
	inline Iterator<std::pair<const std::string, CubemapPtr> > cubemaps() {
		return Iterator<std::pair<const std::string, CubemapPtr> >(cubemap_.begin(), cubemap_.end());
	}
	
	//! Returns an iterator to the meshes
	inline Iterator<std::pair<const std::string, ShaderPtr> > shaders() {
		return Iterator<std::pair<const std::string, ShaderPtr> >(shader_.begin(), shader_.end());
	}
		
	//! Returns an iterator to the meshes
	inline Iterator<std::pair<const std::string, FontPtr> > fonts() {
		return Iterator<std::pair<const std::string, FontPtr> >(font_.begin(), font_.end());
	}
	
	//! Returns an iterator to all listeners
	inline Iterator<EngineListenerPtr> listeners() {
		return Iterator<EngineListenerPtr>(listener_.begin(), listener_.end());
	}
    
    //! Runs the engine through one complete loop.  Note that the engine may
    //! or may not actually do anything on a given loop, depending on the
    //! elapsed time.
    void update();	
	
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
    std::string resolve_path(const std::string& path);
    void update_frame_delta();
	void update_fps();
	void init_systems();
	
	// Map containing engine options
	std::map<std::string, boost::any> option_;
    std::set<std::string> search_folder_;


	// Resource descriptors
	std::map<std::string, SoundPtr> sound_;
    std::map<std::string, MaterialPtr> material_;
    std::map<std::string, MeshPtr> mesh_;
    std::map<std::string, TexturePtr> texture_;
	std::map<std::string, CubemapPtr> cubemap_;
	std::map<std::string, ShaderPtr> shader_;
	std::map<std::string, FontPtr> font_;
	std::map<std::string, GeometryPtr> geometry_;

    NodePtr root_;
	OverlayPtr screen_;
	OverlayPtr focused_overlay_;
	CameraPtr camera_;
	ModulePtr module_;
		
	// Listeners
    std::list<EngineListenerPtr> listener_;
	

	// Engine sub-systems
	GraphicsPtr graphics_;
	ScriptPtr script_;
	InputPtr input_;
	PhysicsPtr physics_;
	AudioPtr audio_;
	NetworkPtr network_;

    // Record-keeping values for timing statistics
    bool running_;
	bool initialized_;
    float frame_delta_;
	float frame_time_;
	float frame_accumulator_;
	uint32_t fps_frame_count_;
	float fps_elapsed_time_;
	size_t auto_name_counter_;
    
    uint32_t prev_time_;
    uint32_t frame_id_;
	uint32_t tick_id_;
};

}


/*
http://!!warningdonottypetoohard--@xoxoalsodonotleavetomorrrowww.stayhere.com
by jilli
*/
