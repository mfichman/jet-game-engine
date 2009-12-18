/*
 * Copyright (c) 2008 Matt Fichman
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

#include <Jet/Registry.hpp>
#include <Jet/Actor.hpp>
#include <Jet/Camera.hpp>
#include <Jet/Entity.hpp>
#include <Jet/Model.hpp>
#include <Jet/Quad.hpp>
#include <Jet/Publisher.hpp>
#include <Jet/Loader.hpp>
#include <map>

namespace Jet {
using namespace std;
using namespace std::tr1;

class Root : public Interface {
public:
    class Listener;
	typedef intrusive_ptr<Root> Ptr;

	static Root::Ptr	    make() { return Root::Ptr(new Root()); }

    // Attributes
	inline Registry::Ptr    registry() { return registry_; }
    inline Loader::Ptr      loader() { return loader_; }    

    Model::Ptr              modelNew(const string& name="");
    Entity::Ptr             entityNew(const string& name="");
    Camera::Ptr             cameraNew(const string& name="");
    Quad::Ptr               quadNew(const string& name="");

    Model::Ptr              model(const string& name);
    Entity::Ptr             entity(const string& name);
    Camera::Ptr             camera(const string& name);
    Quad::Ptr               quad(const string& name);

    inline Camera::Ptr      activeCamera() const { return activeCamera_; }
    void                    activeCamera(Camera::Ptr c);

    // Utility
    Publisher<Listener>&    publisher() const { return publisher_; }

private:
	Root() : registry_(new Registry), loader_(new Loader) {}

    mutable Publisher<Listener> publisher_;
	Registry::Ptr registry_;
    Loader::Ptr loader_;
    Camera::Ptr activeCamera_;
    map<string, Model::Ptr> model_;
    map<string, Entity::Ptr> entity_;
    map<string, Camera::Ptr> camera_;
    map<string, Quad::Ptr> quad_;
};

class Root::Listener : public Interface {
public:
    typedef intrusive_ptr<Root::Listener> Ptr;

    virtual void onModelNew(Model::Ptr)=0;
    virtual void onEntityNew(Entity::Ptr)=0;
    virtual void onCameraNew(Camera::Ptr)=0;
    virtual void onQuadNew(Quad::Ptr)=0;
};

}
