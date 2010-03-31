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

    //! Returns a blueprint node.
    //! @param type the type identifier
    Node* node(const std::string& type) const;

    //! Returns the blueprint component.
    //! @param type the type identifier
    Component* component(const std::string& type) const;

    //! Returns the given mesh object.
    //! @param name the name of the mesh
    Mesh* mesh(const std::string& name) const;

    //! Returns the given texture object.
    //! @param name the name of the texture
    Texture* texture(const std::string& name) const;

    //! Returns a list of visible nodes (after BVH culling).  These are the 
    //! nodes that will be displayed by the renderer.  Node that this list only 
    //! includes the list of parent nodes, not all nodes in the tree.  Thus, 
    //! the renderer should check for children of each node in this list.
    Iterator<NodePtr> renderables() const;

    //! Returns a list of nodes with lighting information.  Note that light
    //! nodes may be culled depending on their distance from the viewer and
    //! potentially an obstacles.
    Iterator<NodePtr> lights() const;

    //! Adds a new template node to the engine
    //! @param type the name of the template
    //! @param node the template node
    void node(const std::string& type, Node* node);

    //! Adds a new template component to the engine
    //! @param type the name of the template
    //! @param component the template component
    void component(const std::string& type, Component* component);

    //! Adds a new mesh to the engine.
    //! @param name the name of the mesh
    //! @param mesh the mesh object
    void mesh(const std::string& name, Mesh* mesh);

    //! Adds a new texture to the engine.
    //! @param name the name of the texture
    //! @param texture the texture object
    void texture(const std::string& name, Texture* texture);

    //! Loads a module into the engine.
    //! @param module the module object
    void module(Object* module);

    //! Loads a module into the engine.
    //! @param path the path to the module
    void module(const std::string& path);

    //! Loads a resource into the engine.
    //! @param path the path to the resource
    void resource(const std::string& path);

    //! Adds a factory to the engine.
    //! @param factory the new factory
    void factory(const std::string& type, Factory* factory);

    //! Adds a handler to the engine.
    //! @param event the type of event to listen for
    //! @param handler the handler to add
    void handler(EngineEvent event, Handler* handler);

private:
#pragma warning(disable:4251)
    NodePtr root_;
    std::map<std::string, FactoryPtr> factory_;
    std::map<std::string, NodePtr> node_;
    std::map<std::string, ComponentPtr> component_;
    std::map<std::string, MeshPtr> mesh_;
    std::map<std::string, TexturePtr> texture_;
    std::set<std::string> resource_;
    std::list<ObjectPtr> module_;
    std::multimap<EngineEvent, HandlerPtr> handler_;
#pragma warning(default:4251)
};

}


/*
http://!!warningdonottypetoohard--@xoxoalsodonotleavetomorrrowww.stayhere.com
by jilli
*/
