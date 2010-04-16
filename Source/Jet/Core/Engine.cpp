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

#include <Jet/Core/Engine.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <boost/filesystem/operations.hpp>
#include <cstdint>

#include <Jet/Core/RenderSystem.hpp>
#include <Jet/Core/ScriptSystem.hpp>

#include <Jet/Core/AudioSource.hpp>
#include <Jet/Core/Camera.hpp>
#include <Jet/Core/Light.hpp>
#include <Jet/Core/Material.hpp>
#include <Jet/Core/Mesh.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/Core/ParticleSystem.hpp>
#include <Jet/Core/QuadChain.hpp>
#include <Jet/Core/QuadSet.hpp>
#include <Jet/Core/RigidBody.hpp>
#include <Jet/Core/Shader.hpp>

#include <Jet/Iterator.hpp>
#include <IL/IL.h>

#define JET_MAX_TIME_LAG 0.5f
#define JET_TIME_STEP 1.0f/60.0f 

using namespace Jet;
using namespace std;
using namespace boost::filesystem;
using namespace boost;

Engine* Engine::create() {
	return new Core::Engine();
}

Core::Engine::Engine() :
    running_(true),
	initialized_(false),
	accumulator_(0) {
        
    root_.reset(new Core::Node(this));
	
    listener(new RenderSystem(this));
	listener(new ScriptSystem(this));
	
#ifdef WINDOWS
	::int64_t counts_per_sec = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
    secs_per_count_ = 1.0f/(float)counts_per_sec;
    prev_time_ = 0;
#endif

    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
        throw runtime_error("IL library version mismatch");
	}
    ilInit();
}

Core::Engine::~Engine() {
}

void Core::Engine::init_systems() {
	initialized_ = true;
	for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
		(*i)->on_init();
	}
}

Jet::Mesh* Core::Engine::mesh(const std::string& name) {
    map<string, Jet::MeshPtr>::iterator i = mesh_.find(name);
    if (i == mesh_.end()) {
        Core::MeshPtr mesh(new Core::Mesh(this, name));
        mesh_.insert(make_pair(name, mesh));
        return mesh.get();
	} else {
		return i->second.get();
	}
}

Jet::Texture* Core::Engine::texture(const std::string& name) {
    map<string,Jet::TexturePtr>::iterator i = texture_.find(name);
    if (i == texture_.end()) {
        Core::TexturePtr texture(new Core::Texture(this, name));
        texture_.insert(make_pair(name, texture));
        return texture.get();
	} else {
		return i->second.get();
	}
}

Jet::Material* Core::Engine::material(const std::string& name) {
    map<string, Jet::MaterialPtr>::iterator i = material_.find(name);
    if (i == material_.end()) {
        Core::MaterialPtr material(new Core::Material(this, name));
        material_.insert(make_pair(name, material));
        return material.get();
	} else {
		return i->second.get();
	}
}

Jet::Shader* Core::Engine::shader(const std::string& name) {
    map<string, Jet::ShaderPtr>::iterator i = shader_.find(name);
    if (i == shader_.end()) {
        Core::ShaderPtr shader(new Core::Shader(this, name));
        shader_.insert(make_pair(name, shader));
        return shader.get();
	} else {
		return i->second.get();
	}
}

std::string Core::Engine::resource_path(const std::string& name) const {
    for (set<string>::const_iterator i = search_folder_.begin(); i != search_folder_.end(); i++) {
		string path = *i + "/" + name;
		ifstream in(path.c_str());
		if (in.good()) {
			return path;
        }
    } 
    throw range_error("Resource not found: " + name);
}

std::string Core::Engine::resolve_path(const std::string& name) {
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

void Core::Engine::tick() {
    static int frames = 0;
    static float elapsed = 0.0f;
	
	if (!initialized_) {
		init_systems();
	}
	
    update_delta();
    
    // Calculate FPS
    elapsed += delta_;
    frames++;
    if (elapsed > 1.0f) {
        cout << frames/elapsed << endl;
        frames = 0;
        elapsed = 0.0f;
    }
    
    accumulator_ += delta_;
    accumulator_ = min(accumulator_, JET_MAX_TIME_LAG);
    while (accumulator_ >= JET_TIME_STEP) {
        // capture input
        for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
            (*i)->on_update();
        }
		accumulator_ -= JET_TIME_STEP;
    }
    
    // Fire render event
    for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->on_render();
    }
}


void Core::Engine::update_delta() {
#ifdef WINDOWS
	::int64_t current_time = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    if (!prev_time_) {
        prev_time_ = current_time;
    }
    delta_ = (current_time - prev_time_) * secs_per_count_;
    prev_time_ = current_time;
#else
#error "Not implemented"
#endif
}
