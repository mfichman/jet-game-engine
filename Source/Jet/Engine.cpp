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
#include <Jet/Material.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <boost/filesystem/operations.hpp>
#include <cstdint>

#define JET_MAX_TIME_LAG 0.5f
#define JET_TIME_STEP 1.0f/60.0f 

using namespace Jet;
using namespace std;
using namespace boost::filesystem;
using namespace boost;

Engine::Engine() :
    running_(true) {
        
    root_.reset(new Node(this));
    
#ifdef WINDOWS
	::int64_t counts_per_sec = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
    secs_per_count_ = 1.0f/(float)counts_per_sec;
    prev_time_ = 0;
#endif
}

Engine::~Engine() {
}

Mesh* Engine::mesh(const std::string& name) {
    map<string, MeshPtr>::iterator i = mesh_.find(name);
    if (i == mesh_.end()) {
        MeshPtr mesh(new Mesh(this, name));
        mesh_.insert(make_pair(name, mesh));
        return mesh.get();
	} else {
		return i->second.get();
	}
}

Texture* Engine::texture(const std::string& name) {
    map<string, TexturePtr>::iterator i = texture_.find(name);
    if (i == texture_.end()) {
        TexturePtr texture(new Texture(this, name));
        texture_.insert(make_pair(name, texture));
        return texture.get();
	} else {
		return i->second.get();
	}
}

Material* Engine::material(const std::string& name) {
    map<string, MaterialPtr>::iterator i = material_.find(name);
    if (i == material_.end()) {
        MaterialPtr material(new Material(this, name));
        material_.insert(make_pair(name, material));
        return material.get();
	} else {
		return i->second.get();
	}
}

Shader* Engine::shader(const std::string& name) {
    map<string, ShaderPtr>::iterator i = shader_.find(name);
    if (i == shader_.end()) {
        ShaderPtr shader(new Shader(this, name));
        shader_.insert(make_pair(name, shader));
        return shader.get();
	} else {
		return i->second.get();
	}
}

Iterator<MeshObjectPtr> Engine::visible_mesh_objects() {
	return Iterator<MeshObjectPtr>(visible_mesh_objects_.begin(), visible_mesh_objects_.end());
}

Iterator<LightPtr> Engine::active_lights() {
	return Iterator<LightPtr>(active_lights_.begin(), active_lights_.end());
}

Iterator<const std::string> Engine::search_folders() const {
    return Iterator<const std::string>(search_folder_.begin(), search_folder_.end());
}

void Engine::resource_loader(const std::string& type, ResourceLoader* loader) {
    resource_loader_[type] = loader;
}

void Engine::object_factory(const std::string& type, ObjectFactory* factory) {
    object_factory_[type] = factory;    
}

void Engine::resource(const std::string& name) {
	size_t start = name.rfind(".");
	if (start == string::npos) {
		return;
	}
    std::string ext = name.substr(start, string::npos);
    map<string, ResourceLoaderPtr>::iterator i = resource_loader_.find(ext);
    if (i != resource_loader_.end()) {
		i->second->resource(name);
    } else {
        throw runtime_error("No loader for: " + name); 
    }
}

std::string Engine::resource_path(const std::string& name) {
    for (Iterator<const string> j = search_folders(); j; j++) {
		string folder = *j;
		string path = folder + "/" + name;
		ifstream in(path.c_str());
		if (in.good()) {
			return path;
        }
    } 
    throw range_error("Resource not found: " + name);
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

void Engine::listener(EngineListener* listener) {
    listener_.push_back(listener);
}

void Engine::search_folder(const std::string& dir) {
    search_folder_.insert(resolve_path(dir));
}

std::string Engine::resolve_path(const std::string& name) {
    path file =  initial_path() / name;
    path result;
    for (path::iterator i = file.begin(); i != file.end(); i++) {
        if (*i == "..") {
            if (is_symlink(result)) {
                result /= *i;
            } else if (result.filename() == "..") {
                result /= *i;
            } else {
                result = result.parent_path();
            }
        } else if (*i == ".") {
            // ignore
        } else {
            result /= *i;
        }
    }
    return result.string();
}

void Engine::tick() {
    static int frames = 0;
    static float elapsed = 0.0f;
    
    
    float d = delta();
    
    elapsed += d;
    frames++;
    if (elapsed > 1.0f) {
        cout << frames/elapsed << endl;
        frames = 0;
        elapsed = 0.0f;
    }
    
    accumulator_ += d;
    accumulator_ = min(accumulator_, JET_MAX_TIME_LAG);
    while (accumulator_ >= JET_TIME_STEP) {
        // capture input
        for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
            (*i)->on_update();
        }
    }
    
    // Traverse the scene graph
    generate_render_list(root_);

    // Fire pre-render event
    for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->on_pre_render();
    }
    
    // Fire render event
    for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->on_render();
    }
    
     // Fire post-render event
    for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->on_post_render();
    }
    
    
    // Clear render lists
    visible_mesh_objects_.clear();
    visible_particle_systems_.clear();
    visible_quad_sets_.clear();
    visible_quad_chains_.clear();
    active_lights_.clear();
    audible_audio_sources_.clear();
    
}

void Engine::generate_render_list(NodePtr node) {
    // Update the transformation matrix for this node, if it
    // was modified
    if (node->parent()) {
        const Matrix& parent = node->parent()->matrix();
        node->matrix(Matrix(node->rotation(), node->position()) * parent);
    } else {
        node->matrix(Matrix(node->rotation(), node->position()));
    }
    
    for (Iterator<ObjectPtr> i = node->objects(); i; i++) {
		const type_info& type = typeid(**i);
		if (type == typeid(Node)) {
			generate_render_list(static_cast<Node*>(i->get()));
		} else if (type == typeid(MeshObject)) {
			visible_mesh_objects_.push_back(static_cast<MeshObject*>(i->get()));
		} else if (type == typeid(Light)) {
			active_lights_.push_back(static_cast<Light*>(i->get()));
		}
    }
}

real_t Engine::delta() {
#ifdef WINDOWS
	::int64_t current_time = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    if (!prev_time_) {
        prev_time_ = current_time;
    }
    real_t delta = (current_time - prev_time_) * secs_per_count_;
    prev_time_ = current_time;
    
    return delta;
#else
#error "Not implemented"
#endif
}
