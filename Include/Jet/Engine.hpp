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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Node.hpp>
#include <Jet/Loader.hpp>
#include <Jet/Factory.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Handler.hpp>
#include <list>
#include <map>
#include <set>

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
    ~Engine();

    //! Returns the root scene node.  This node can be used to add 
    //! other objects to the scene.
    inline Node* root() const { 
        return root_.get(); 
    }
        
    //! Returns true if the engine is running
    inline bool running() const {
        return running_;
    }
    
    //! Returns a blueprint node.
    //! @param type the type identifier
    Node* node(const std::string& type, bool create=true) const;

    //! Returns the blueprint component.
    //! @param type the type identifier
    Component* component(const std::string& type, bool create=true) const;

    //! Returns the given mesh object.
    //! @param name the name of the mesh.
    //! @param load whether or not to load the resource, if it is not loaded.
    Mesh* mesh(const std::string& name, bool load=true) const;

    //! Returns the given texture object.
    //! @param name the name of the texture
    //! @param load whether or not to load the resource, if it is not loaded.
    Texture* texture(const std::string& name, bool load=true) const;
    
     //! Loads an object into the engine using the factory for the given type.
    //! @param type the object type to load
    Object* object(const std::string& type);

    //! Returns a list of visible nodes (after BVH culling).  These are the 
    //! nodes that will be displayed by the renderer.  Node that this list only 
    //! includes the list of parent nodes, not all nodes in the tree.  Thus, 
    //! the renderer should check for children of each node in this list.
    Iterator<const std::pair<NodePtr, ComponentPtr>> renderables() const;

    //! Returns a list of nodes with lighting information.  Note that light
    //! nodes may be culled depending on their distance from the viewer and
    //! potentially an obstacles.
    Iterator<const std::pair<NodePtr, ComponentPtr>> lights() const;
    
    //! Returns a list of folders representing the search path for loading
    //! resources.
    Iterator<const std::string> folders() const;

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
    void loader(const std::string& type, Loader* loader);
    
    //! Adds a factory to the engine.  Factories are used to load objects
    //! needed by the engine.  Usually, objects loaded are controllers for
    //! scene nodes, although other types of objects can be requested and
    //! loaded if the plugin registering the factory can handle that type of
    //! object.
    void factory(const std::string& type, Factory* factory);

    //! Adds a handler to the engine.
    //! @param handler the handler, which listens for engine events.
    void handler(Handler* handler);
    
    //! Adds a folder to the search path for loading resources.  Resources
    //! will be loaded automatically.
    //! @param folder the folder to add
    void folder(const std::string& path);
    
    //! Sets whether or not the engine is running.
    //! @param running false to stop the engine
    inline void running(bool running) {
        running_ = running;
    }
    
    //! Runs the engine through one complete loop.  Note that the engine may
    //! or may not actually do anything on a given loop, depending on the
    //! elapsed time.
    void tick();

private:
    real_t delta();
    std::string resolve_path(const std::string& path);
    void generate_render_list(NodePtr node);
    
#pragma warning(disable:4251)
    NodePtr root_;
    std::map<std::string, LoaderPtr> loader_;
    std::map<std::string, FactoryPtr> factory_;
    std::map<std::string, NodePtr> node_;
    std::map<std::string, ComponentPtr> component_;
    std::map<std::string, MeshPtr> mesh_;
    std::map<std::string, TexturePtr> texture_;
    std::set<std::string> resource_;
    std::list<HandlerPtr> handler_;
    std::set<std::string> folder_;
    std::list<std::pair<NodePtr, ComponentPtr>> renderables_;
    std::list<std::pair<NodePtr, ComponentPtr>> lights_;
    bool running_;
    real_t accumulator_;
#ifdef WINDOWS
    float secs_per_count_;
    int64_t prev_time_;
#endif
    
#pragma warning(default:4251)
};

}


/*
http://!!warningdonottypetoohard--@xoxoalsodonotleavetomorrrowww.stayhere.com
by jilli
*/
