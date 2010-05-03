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
#include <SDL/SDL_image.h>

#include <Jet/Core/RenderSystem.hpp>
#include <Jet/Core/ScriptSystem.hpp>
#include <Jet/Core/PhysicsSystem.hpp>
#include <Jet/Core/InputSystem.hpp>

#include <Jet/Core/AudioSource.hpp>
#include <Jet/Core/AudioSystem.hpp>
#include <Jet/Core/Camera.hpp>
#include <Jet/Core/Sound.hpp>
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
#include <Jet/Core/Overlay.hpp>

#include <Jet/Iterator.hpp>
#include <fstream>

#define JET_MAX_TIME_LAG 0.5f

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
	fps_frame_count_(0),
	fps_elapsed_time_(0.0f),
	frame_delta_(0.0f),
	frame_time_(0.0f) {
		
	cout << "Starting kernel..." << endl;
		
	// Add some default search folders
	search_folder(".");
	search_folder("..");
	
	// Default options
	option("simulation_speed", 1.0f);
        
	// Create the root node of the scene graph
    root_ = new Core::Node(this);
	overlay_ = new Core::Overlay(this);
	
	// Create subsystems and register them
	render_system_ = new RenderSystem(this);
	script_system_ = new ScriptSystem(this);
	physics_system_ = new PhysicsSystem(this);
	input_system_ = new InputSystem(this);
	audio_system_ = new AudioSystem(this);
	
	// Platform-dependent timer code
#ifdef WINDOWS
	::int64_t counts_per_sec = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
    secs_per_count_ = 1.0f/(float)counts_per_sec;
    prev_time_ = 0;
#else
	prev_time_.tv_usec = 0;
	prev_time_.tv_sec = 0;
#endif

	// Initialize SDL image library
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
}

Core::Engine::~Engine() {
	if (module_) {
		module_->on_destroy();
	}

	// Free the scene graph, the free all resources
	root_.reset();
	module_.reset();
	mesh_.clear();
	texture_.clear();
	shader_.clear();
	material_.clear();
	
	cout << "Shutting down" << endl;
	listener_.clear();

	// Quit the SDL image library
	IMG_Quit();
}

void Core::Engine::init_systems() {
	initialized_ = true;
	for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
		(*i)->on_init();
	}
}

Jet::Font* Core::Engine::font(const std::string& name) {
	throw runtime_error("Not implemented");
}

Jet::Sound* Core::Engine::sound(const std::string& name) {
	map<string, Jet::SoundPtr>::iterator i = sound_.find(name);
    if (i == sound_.end()) {
        Core::SoundPtr sound(new Core::Sound(this, name));
        sound_.insert(make_pair(name, sound));
        return sound.get();
	} else {
		return i->second.get();
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
	// Initialize the engine systems if this is the first tick
	if (!initialized_) {
		init_systems();
	}
	
	// Update the delta since the last tick
    update_frame_delta();
	update_fps();

	input_system_->on_update();
    
	// Run the fixed-time step portion of the game by calling on_update when
	physics_system_->step();
    
    // Fire render event
    for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->on_render();
    }
	
	if (module_) {
		module_->on_render();
	}
	
	frame_time_ += frame_delta_ * option<real_t>("simulation_speed");
}

void Core::Engine::update_fps() {
	// This is a rough calculation of the number of frames per second.
    fps_elapsed_time_ += frame_delta_;
    fps_frame_count_++;
    if (fps_elapsed_time_ > 0.1f) {
        cout << fps_frame_count_/fps_elapsed_time_ << "\t";
		cout << script_system_->memory_usage() << "K" << endl;
        fps_frame_count_ = 0;
        fps_elapsed_time_ = 0.0f;
    }
}


void Core::Engine::update_frame_delta() {
#ifdef WINDOWS
	::int64_t current_time = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    if (!prev_time_) {
        prev_time_ = current_time;
    }
    frame_delta_ = (current_time - prev_time_) * secs_per_count_;
    prev_time_ = current_time;
#else
	timeval current_time;
	gettimeofday(&current_time, 0);
	if (prev_time_.tv_sec == 0 && prev_time_.tv_usec == 0) {
		prev_time_ = current_time;
	}

	time_t frame_delta_sec = current_time.tv_sec - prev_time_.tv_sec;
	long frame_delta_usec = current_time.tv_usec - prev_time_.tv_usec;

	frame_delta_ = (float)frame_delta_sec + (float)frame_delta_usec/1000000.0f;
	prev_time_ = current_time;

#endif
}
