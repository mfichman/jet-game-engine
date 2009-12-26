/*
 * Copyright (c) 2009 Matt Fichman
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

#include <Jet/Registry.hpp>
#include <Jet/Actor.hpp>
#include <Jet/Camera.hpp>
#include <Jet/Entity.hpp>
#include <Jet/Model.hpp>
#include <Jet/Quad.hpp>
#include <Jet/Speaker.hpp>
#include <Jet/Listener.hpp>
#include <Jet/Cloud.hpp>
#include <Jet/Publisher.hpp>
#include <Jet/Loader.hpp>
#include <map>

namespace Jet {
using namespace std;
using namespace std::tr1;

class Root : public Interface {
public:
    class Observer;
	typedef intrusive_ptr<Root> Ptr;

	static Root::Ptr	    make() { return Root::Ptr(new Root()); }

    // Attributes
	inline Registry::Ptr    registry() { return registry_; }
    inline Loader::Ptr      loader() { return loader_; }    

    Model::Ptr              modelNew(const string& name="");
    Entity::Ptr             entityNew(const string& name="");
    Camera::Ptr             cameraNew(const string& name="");
    Quad::Ptr               quadNew(const string& name="");
    Speaker::Ptr            speakerNew(const string& name="");
    Listener::Ptr           listenerNew(const string& name="");
    Cloud::Ptr              cloudNew(const string& name="");

    inline Model::Ptr       model(const string& name) { return model_[name]; }
    inline Entity::Ptr      entity(const string& name) { return entity_[name]; }
    inline Camera::Ptr      camera(const string& name) { return camera_[name]; }
    inline Quad::Ptr        quad(const string& name) { return quad_[name]; }
    inline Listener::Ptr    listener(const string& name) { return listener_[name]; }
    inline Cloud::Ptr       cloud(const string& name) { return cloud_[name]; }

    inline Camera::Ptr      activeCamera() const { return activeCamera_; }
    void                    activeCamera(Camera::Ptr c);
    inline Listener::Ptr    activeListener() const { return activeListener_; }
    void                    activeListener(Listener::Ptr l);
    inline float            time() const { return time_; }
    void                    timeInc(float t);

    // Utility
    Publisher<Observer>&    publisher() const { return publisher_; }

private:
	Root() : registry_(new Registry), loader_(new Loader(this)) {}

    template <typename T>
    inline typename T::Ptr objectNew(
        const string&, 
        map<string, typename T::Ptr>&, 
        void (Root::Observer::*)(typename T::Ptr)); 

    mutable Publisher<Observer> publisher_;
	Registry::Ptr registry_;
    Loader::Ptr loader_;
    Camera::Ptr activeCamera_;
    Listener::Ptr activeListener_;
    float time_;
    map<string, Model::Ptr> model_;
    map<string, Entity::Ptr> entity_;
    map<string, Camera::Ptr> camera_;
    map<string, Quad::Ptr> quad_;
    map<string, Speaker::Ptr> speaker_;
    map<string, Listener::Ptr> listener_;
    map<string, Cloud::Ptr> cloud_;
};

class Root::Observer : public Interface {
public:
    typedef intrusive_ptr<Root::Observer> Ptr;

    virtual void onModelNew(Model::Ptr)=0;
    virtual void onEntityNew(Entity::Ptr)=0;
    virtual void onCameraNew(Camera::Ptr)=0;
    virtual void onQuadNew(Quad::Ptr)=0;
    virtual void onListenerNew(Listener::Ptr)=0;
    virtual void onSpeakerNew(Speaker::Ptr)=0;
    virtual void onCloudNew(Cloud::Ptr)=0;
    virtual void onTime()=0;
};
}
