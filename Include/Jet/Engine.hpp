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
#include <Jet/Node.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/ParticleSystem.hpp>
#include <Jet/Material.hpp>
#include <Jet/ResourceLoader.hpp>
#include <Jet/ObjectFactory.hpp>
#include <Jet/QuadSet.hpp>
#include <Jet/QuadChain.hpp>
#include <Jet/AudioSource.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Light.hpp>
#include <Jet/RigidBody.hpp>
#include <list>
#include <map>
#include <set>
#include <boost/any.hpp>

namespace Jet {

//! This is the main engine class.  This object is the parent of all other
//! objects in the game engine.
//! @class Engine
//! @brief Main engine class.
class JETAPI Engine : public Object {
public:    
    //! Constructor
    Engine();

    //! Destructor
    virtual ~Engine();

    //! Returns the root scene node.  This node can be used to add 
    //! other objects to the scene.
    inline Node* root() const { 
        return root_.get(); 
    }
        
    //! Returns true if the engine is running
    inline bool running() const {
        return running_;
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

    //! Returns the material with the given name.
    //! @param name the name of the material
    Material* material(const std::string& type);

    //! Returns the mesh with the given name.
    //! @param name the name of the mesh.
    Mesh* mesh(const std::string& name);

    //! Returns the given texture descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    Texture* texture(const std::string& name);
    
    //! Returns the given shader descriptor.  This function will attempt to
    //! load the underlying resource if load is set to true.
    //! @param name the name of the texture
    Shader* shader(const std::string& name);
    
    //! Returns the full path to the file using the given resource name.
    //! @param name the name of the resource
    std::string resource_path(const std::string& name);

    //! Loads a module into the engine.
    //! @param path the path to the module
    void module(const std::string& path);

    //! Loads a resource into the engine.
    //! @param path the path to the resource
    void resource(const std::string& path);

    //! Adds a loader to the engine.  Loaders are used to load resources
    //! needed by the engine.  They can be registered to handle a specific
    //! type of file.
    //! @param loader the new loader
    void resource_loader(const std::string& type, ResourceLoader* loader);
    
    //! Adds a factory to the engine.  Factories are used to load objects
    //! needed by the engine.  Usually, objects loaded are controllers for
    //! scene nodes, although other types of objects can be requested and
    //! loaded if the plugin registering the factory can handle that type of
    //! object.
    void object_factory(const std::string& type, ObjectFactory* factory);

    //! Adds a listener, which listens for engine events.
    //! @param listener the engine listener.
    void listener(EngineListener* listener);
    
    //! Adds a folder to the search path for loading resources.  Resources
    //! will be loaded automatically.
    //! @param folder the folder to add
    void search_folder(const std::string& path);
    
    //! Sets whether or not the engine is running.
    //! @param running false to stop the engine
    inline void running(bool running) {
        running_ = running;
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
    
    //! Runs the engine through one complete loop.  Note that the engine may
    //! or may not actually do anything on a given loop, depending on the
    //! elapsed time.
    void tick();
    
    //! Returns an iterator over the visible meshes (after BVH culling).
    //! These are the meshes that will be displayed by the renderer.
    Iterator<MeshObjectPtr> visible_mesh_objects();
    
    //! Returns an iterator over the visible particle systems (after BFH
    //! culling).  These are the particle systems that will be displayed by
    //! the renderer.
    Iterator<ParticleSystemPtr> visible_particle_systems();
    
    //! Returns an iterator over the visible quads (after BVH culling).
    Iterator<QuadSetPtr> visible_quad_sets();
    
    //! Returns an iterator over the visible quad chains (after BVH culling).
    Iterator<QuadChainPtr> visible_quad_chains();
    
    //! Returns active lights.
    Iterator<LightPtr> active_lights();
    
    //! Returns an iterator over all active rigid bodies in the scene.
    Iterator<RigidBodyPtr> active_rigid_bodies();
    
    //! Returns an iterator over all audible audio sources that are within
    //! hearing range of the camera.
    Iterator<AudioSourcePtr> audible_audio_sources();
    
    //! Returns an iterator over the folders representing the search path for
    //! loading resources.
    Iterator<const std::string> search_folders() const;

private:
    real_t delta();
    std::string resolve_path(const std::string& path);
    void generate_render_list(NodePtr node);
    
#pragma warning(disable:4251)
    NodePtr root_;
    std::map<std::string, ResourceLoaderPtr> resource_loader_;
    std::map<std::string, ObjectFactoryPtr> object_factory_;
    std::map<std::string, MaterialPtr> material_;
    std::map<std::string, MeshPtr> mesh_;
    std::map<std::string, TexturePtr> texture_;
	std::map<std::string, ShaderPtr> shader_;
    std::list<EngineListenerPtr> listener_;
    std::set<std::string> search_folder_;
    std::vector<MeshObjectPtr> visible_mesh_objects_;
    std::vector<ParticleSystemPtr> visible_particle_systems_;
    std::vector<QuadSetPtr> visible_quad_sets_;
    std::vector<QuadChainPtr> visible_quad_chains_;
    std::vector<LightPtr> active_lights_;
    std::vector<RigidBodyPtr> active_rigid_bodies_;
    std::vector<AudioSourcePtr> audible_audio_sources_;
	std::map<std::string, boost::any> option_;
    
    bool running_;
    real_t accumulator_;
#ifdef WINDOWS
    float secs_per_count_;
    int64_t prev_time_;
#endif
    
#pragma warning(default:4251)
};

//! Listens for engine events.  Examples include on_pre_render (called before
//! rendering), on_post_render (called after rendering), on_render (called
//! during rendering), and on_update (called with each tick of the physics
//! engine.
//! @class EngineListener
//! @brief Interface for handling engine events.
class JETAPI EngineListener : public Object {
public: 
    //! Destructor.
    virtual ~EngineListener() {}

    //! Called for each physics update.
    virtual void on_update() {}
    
    //! Called before each frame.
    virtual void on_pre_render() {}
    
    //! Called during each frame.
    virtual void on_render() {}
    
    //! Called after each frame.
    virtual void on_post_render() {}
};

}


/*
http://!!warningdonottypetoohard--@xoxoalsodonotleavetomorrrowww.stayhere.com
by jilli
*/
