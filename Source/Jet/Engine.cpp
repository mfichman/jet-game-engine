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

#include <Jet/Engine.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using namespace Jet;
using namespace std;

Engine::Engine() {
    root_.reset(new Node());
    root_->engine_ = this;
}

Engine::~Engine() {
}

Node* Engine::node(const std::string& type) const {
    map<string, NodePtr>::const_iterator i = node_.find(type);
    if (i == node_.end()) {
        throw runtime_error("Blueprint not found");
    }
    return i->second.get();
}

Component* Engine::component(const std::string& type) const {
    map<string, ComponentPtr>::const_iterator i = component_.find(type);
    if (i == component_.end()) {
        throw runtime_error("Blueprint not found");
    }
    return i->second.get();
}

Mesh* Engine::mesh(const std::string& type) const {
    map<string, MeshPtr>::const_iterator i = mesh_.find(type);
    if (i != mesh_.end()) {
        throw runtime_error("Mesh not found");
    }
    return i->second.get();
}

Texture* Engine::texture(const std::string& name) const {
    map<string, TexturePtr>::const_iterator i = texture_.find(name);
    if (i != texture_.end()) {
        throw runtime_error(string("Texture not found: ") + name);
    }
    return i->second.get();
}

void Engine::loader(const std::string& type, Loader* loader) {
    loader_[type] = loader;
}

void Engine::factory(const std::string& type, Factory* factory) {
    factory_[type] = factory;    
}

void Engine::node(const std::string& type, Node* node) {
    node_[type] = node;
}

void Engine::component(const std::string& type, Component* component) {
    component_[type] = component;
}

void Engine::mesh(const std::string& name, Mesh* mesh) {
    mesh_[name] = mesh;
}

void Engine::texture(const std::string& name, Texture* texture) {
    texture_[name] = texture;
}

void Engine::resource(const std::string& name) {
    std::string ext = name.substr(name.rfind("."), string::npos);
    map<string, LoaderPtr>::iterator i = loader_.find(ext);
    if (i != loader_.end()) {
        i->second->create(name);
    } else {
        throw runtime_error(string("No loader for: ") + name); 
    }
}

 Object* Engine::object(const std::string& type) {
    std::string ext = type.substr(type.rfind("."), string::npos);
    map<string, FactoryPtr>::iterator i = factory_.find(ext);
    if (i != factory_.end()) {
        return i->second->create(type);
    } else {
        throw runtime_error(string("No factory for: ") + type);
    }
 }

void Engine::module(Object* module) {
    module_.push_back(module);
}

void Engine::module(const std::string& path) {
#ifdef WINDOWS
	std::string file = "./" + path + ".dll";
	HMODULE handle = LoadLibrary(file.c_str());
	if (!handle) {
		throw runtime_error("Could not load system '" + path + "'.");
	}
	load_function_t load = (load_function_t)GetProcAddress(handle, "load");
	if (!load) {
		throw runtime_error("Could not load system symbols for '" + path + "'.");
	}
#else 
#ifdef DARWIN
	std::string file = "./lib" + path + ".dylib";
#else
	std::string file = "./lib" + path + ".so";
#endif
	void* handle = dlopen(file.c_str(), RTLD_LAZY);
	if (!handle) {
		throw runtime_error("Could not load system '" + path + "'.");
	}
	load_function_t load = (load_function_t)dlsym(handle, "load");
	if (!load) {
		runtime_error("Could not load system symbols for '" + path + "'.");
	}
#endif
	load(this);
}

void Engine::handler(Handler* handler) {
    handler_.push_back(handler);
}
