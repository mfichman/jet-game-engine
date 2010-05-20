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
#include <boost/lexical_cast.hpp>
#include <SDL/SDL_image.h>

#include <Jet/Core/Overlay.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/Core/Node.hpp>
#include <Jet/Core/QuadChain.hpp>
#include <Jet/Core/QuadSet.hpp>
#include <Jet/Core/Camera.hpp>
#include <Jet/Core/Light.hpp>
#include <Jet/Iterator.hpp>

#include <Jet/Sockets/NetworkSystem.hpp>
#include <Jet/OpenGL/RenderSystem.hpp>
#include <Jet/Lua/ScriptSystem.hpp>
#include <Jet/SDL/InputSystem.hpp>
#include <Jet/Bullet/PhysicsSystem.hpp>
#include <Jet/FMOD/AudioSystem.hpp>
#include <fstream>

#define JET_MAX_TIME_LAG 0.5f

using namespace Jet;
using namespace std;
using namespace boost::filesystem;
using namespace boost;

Engine* Engine::create() {
	Core::EnginePtr engine = new Core::Engine();
	engine->refcount_inc();
	engine->network(new Sockets::NetworkSystem(engine.get()));
	engine->renderer(new OpenGL::RenderSystem(engine.get()));
	engine->script(new Lua::ScriptSystem(engine.get()));
	engine->input(new SDL::InputSystem(engine.get()));
	engine->physics(new Bullet::PhysicsSystem(engine.get()));
	engine->audio(new FMOD::AudioSystem(engine.get()));
	return engine.get();
}

Core::Engine::Engine() :
    running_(true),
	initialized_(false),
	frame_delta_(0.0f),
	frame_time_(0.0f),
	fps_frame_count_(0),
	fps_elapsed_time_(0.0f),
	auto_name_counter_(0) {
		
	cout << "Starting kernel" << endl;
	
	option("engine_build", string("Jet Game Engine 2.0.1 "__DATE__" "__TIME__));
		
	// Add some default search folders
	search_folder(".");
	search_folder("..");
	
	// Default options
	option("simulation_speed", 1.0f);
	option("stat_fps", 0.0f);
	option("stat_memory", 0.0f);
        
	// Create the root node of the scene graph
    root_ = new Core::Node(this);
	screen_ = new Core::Overlay(this);
	
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
	screen_.reset();
	focused_overlay_.reset();
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
	map<string, Jet::FontPtr>::iterator i = font_.find(name);
    if (i == font_.end()) {
        Jet::FontPtr font(renderer_->font(name));
        font_.insert(make_pair(name, font));
        return font.get();
	} else {
		return i->second.get();
	}
}

Jet::Sound* Core::Engine::sound(const std::string& name) {
	map<string, Jet::SoundPtr>::iterator i = sound_.find(name);
    if (i == sound_.end()) {
        Jet::SoundPtr sound(audio_->sound(name));
        sound_.insert(make_pair(name, sound));
        return sound.get();
	} else {
		return i->second.get();
	}
}


Jet::Mesh* Core::Engine::mesh(const std::string& name) {
	if (name.empty()) {
		string name = "__" + lexical_cast<string>(auto_name_counter_++);
		Jet::MeshPtr mesh(renderer_->mesh(name));
		mesh_.insert(make_pair(name, mesh));
		return mesh.get();
	}
	
    map<string, Jet::MeshPtr>::iterator i = mesh_.find(name);
    if (i == mesh_.end()) {
        Jet::MeshPtr mesh(renderer_->mesh(name));
        mesh_.insert(make_pair(name, mesh));
        return mesh.get();
	} else {
		return i->second.get();
	}
}

Jet::Geometry* Core::Engine::geometry(const std::string& name) {
    map<string, Jet::GeometryPtr>::iterator i = geometry_.find(name);
    if (i == geometry_.end()) {
        Jet::GeometryPtr geometry(physics_->geometry(name));
        geometry_.insert(make_pair(name, geometry));
        return geometry.get();
	} else {
		return i->second.get();
	}
}

Jet::Mesh* Core::Engine::mesh(Jet::Mesh* parent) {
	string name = "__" + lexical_cast<string>(auto_name_counter_++);
	
    Jet::MeshPtr mesh(renderer_->mesh(name, parent));
    mesh_.insert(make_pair(name, mesh));
    return mesh.get();
}

Jet::Texture* Core::Engine::texture(const std::string& name) {
    map<string, Jet::TexturePtr>::iterator i = texture_.find(name);
    if (i == texture_.end()) {
        Jet::TexturePtr texture(renderer_->texture(name));
        texture_.insert(make_pair(name, texture));
        return texture.get();
	} else {
		return i->second.get();
	}
}

Jet::Material* Core::Engine::material(const std::string& name) {
    map<string, Jet::MaterialPtr>::iterator i = material_.find(name);
    if (i == material_.end()) {
        Jet::MaterialPtr material(renderer_->material(name));
        material_.insert(make_pair(name, material));
        return material.get();
	} else {
		return i->second.get();
	}
}

Jet::Shader* Core::Engine::shader(const std::string& name) {
    map<string, Jet::ShaderPtr>::iterator i = shader_.find(name);
    if (i == shader_.end()) {
        Jet::ShaderPtr shader(renderer_->shader(name));
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

void Core::Engine::update() {
	// Initialize the engine systems if this is the first tick
	if (!initialized_) {
		init_systems();
	}
	
	// Update the delta since the last tick
    update_frame_delta();
	update_fps();
    
	// Run the tick callback
	for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
		(*i)->on_update();
	}
	if (module_) {
		module_->on_update(frame_delta());
	}
	static_cast<Core::Node*>(root())->update();
	static_cast<Core::Overlay*>(screen())->update();
    
    // Fire render event
    for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->on_render();
	}
	if (module_) {
		module_->on_render();
	}
	
	frame_time_ += frame_delta_ * option<float>("simulation_speed");
}

void Core::Engine::update_fps() {
	// This is a rough calculation of the number of frames per second.
    fps_elapsed_time_ += frame_delta_;
    fps_frame_count_++;
    if (fps_elapsed_time_ > 0.1f) {
		option("stat_fps", fps_frame_count_/fps_elapsed_time_);
		option("stat_memory", (float)script_->memory_usage());
        fps_frame_count_ = 0;
        fps_elapsed_time_ = 0.0f;
    }
}


void Core::Engine::update_frame_delta() {
	// Query the counter, and initialize it.  If this is the first time the
	// function is being called, set the "previous" time equal to the current
	// time so that there is not a huge lag for the first frame.
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

void Core::Engine::delete_mesh(const std::string& name) {
	mesh_.erase(name);	
}
