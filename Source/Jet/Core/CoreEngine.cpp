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

#include <Jet/Core/CoreEngine.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#include <SDL/SDL_image.h>

#include <Jet/Core/CoreOverlay.hpp>
#include <Jet/Core/CoreMeshObject.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Core/CoreQuadChain.hpp>
#include <Jet/Core/CoreQuadSet.hpp>
#include <Jet/Core/CoreCamera.hpp>
#include <Jet/Core/CoreLight.hpp>
#include <Jet/Types/Iterator.hpp>

#include <Jet/Network/BSockNetwork.hpp>
#include <Jet/Graphics/OpenGLGraphics.hpp>
#include <Jet/Script/LuaScript.hpp>
#include <Jet/Input/SDLInput.hpp>
#include <Jet/Physics/BulletPhysics.hpp>
#include <Jet/Audio/FMODAudio.hpp>
#include <fstream>
#include <memory>

#define JET_MAX_TIME_LAG 0.5f

using namespace Jet;
using namespace std;
using namespace boost::filesystem;
using namespace boost;

Engine* Engine::create() {
	auto_ptr<CoreEngine> engine(new CoreEngine());
    engine->network(new BSockNetwork(engine.get()));
	engine->graphics(new OpenGLGraphics(engine.get()));
	engine->script(new LuaScript(engine.get()));
	engine->input(new SDLInput(engine.get()));
	engine->physics(new BulletPhysics(engine.get()));
	engine->audio(new FMODAudio(engine.get()));
	return engine.release();
}

CoreEngine::CoreEngine() :
    running_(true),
	initialized_(false),
	frame_delta_(0.0f),
	frame_time_(0.0f),
	fps_frame_count_(0),
	fps_elapsed_time_(0.0f),
	auto_name_counter_(0),
    frame_id_(0) {
		
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
    root_ = new CoreNode(this);
	screen_ = new CoreOverlay(this);
	
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

CoreEngine::~CoreEngine() {
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
	cubemap_.clear();
	shader_.clear();
	material_.clear();
	
	cout << "Shutting down" << endl;
	listener_.clear();

	// Quit the SDL image library
	IMG_Quit();
}

void CoreEngine::init_systems() {
	initialized_ = true;
	for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
		(*i)->on_init();
	}
}

Font* CoreEngine::font(const std::string& name) {
	map<string, FontPtr>::iterator i = font_.find(name);
    if (i == font_.end()) {
        FontPtr font(graphics_->font(name));
        font_.insert(make_pair(name, font));
        return font.get();
	} else {
		return i->second.get();
	}
}

Sound* CoreEngine::sound(const std::string& name) {
	map<string, SoundPtr>::iterator i = sound_.find(name);
    if (i == sound_.end()) {
        SoundPtr sound(audio_->sound(name));
        sound_.insert(make_pair(name, sound));
        return sound.get();
	} else {
		return i->second.get();
	}
}


Mesh* CoreEngine::mesh(const std::string& name) {
	if (name.empty()) {
		string name = "__" + lexical_cast<string>(auto_name_counter_++);
		MeshPtr mesh(graphics_->mesh(name));
		mesh_.insert(make_pair(name, mesh));
		return mesh.get();
	}
	
    map<string, MeshPtr>::iterator i = mesh_.find(name);
    if (i == mesh_.end()) {
        MeshPtr mesh(graphics_->mesh(name));
        mesh_.insert(make_pair(name, mesh));
        return mesh.get();
	} else {
		return i->second.get();
	}
}

Geometry* CoreEngine::geometry(const std::string& name) {
    map<string, GeometryPtr>::iterator i = geometry_.find(name);
    if (i == geometry_.end()) {
        GeometryPtr geometry(physics_->geometry(name));
        geometry_.insert(make_pair(name, geometry));
        return geometry.get();
	} else {
		return i->second.get();
	}
}

Mesh* CoreEngine::mesh(Mesh* parent) {
	string name = "__" + lexical_cast<string>(auto_name_counter_++);
	
    MeshPtr mesh(graphics_->mesh(name, parent));
    mesh_.insert(make_pair(name, mesh));
    return mesh.get();
}

Texture* CoreEngine::texture(const std::string& name) {
    map<string, TexturePtr>::iterator i = texture_.find(name);
    if (i == texture_.end()) {
        TexturePtr texture(graphics_->texture(name));
        texture_.insert(make_pair(name, texture));
        return texture.get();
	} else {
		return i->second.get();
	}
}

Cubemap* CoreEngine::cubemap(const std::string& name) {
    map<string, CubemapPtr>::iterator i = cubemap_.find(name);
    if (i == cubemap_.end()) {
        CubemapPtr cubemap(graphics_->cubemap(name));
        cubemap_.insert(make_pair(name, cubemap));
        return cubemap.get();
	} else {
		return i->second.get();
	}
}


Material* CoreEngine::material(const std::string& name) {
    map<string, MaterialPtr>::iterator i = material_.find(name);
    if (i == material_.end()) {
        MaterialPtr material(graphics_->material(name));
        material_.insert(make_pair(name, material));
        return material.get();
	} else {
		return i->second.get();
	}
}

Shader* CoreEngine::shader(const std::string& name) {
    map<string, ShaderPtr>::iterator i = shader_.find(name);
    if (i == shader_.end()) {
        ShaderPtr shader(graphics_->shader(name));
        shader_.insert(make_pair(name, shader));
        return shader.get();
	} else {
		return i->second.get();
	}
}


std::string CoreEngine::resource_path(const std::string& name) const {
    for (set<string>::const_iterator i = search_folder_.begin(); i != search_folder_.end(); i++) {
		string path = *i + "/" + name;
		ifstream in(path.c_str());
		if (in.good()) {
			return path;
        }
    } 
    throw range_error("Resource not found: " + name);
}

std::string CoreEngine::resolve_path(const std::string& name) {
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

void CoreEngine::update() {
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
	static_cast<CoreNode*>(root())->update();
	static_cast<CoreOverlay*>(screen())->update();
    
    // Fire render event
    for (list<EngineListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
        (*i)->on_render();
	}
	if (module_) {
		module_->on_render();
	}
	
	frame_time_ += frame_delta_ * option<float>("simulation_speed");
    frame_id_++;
}

void CoreEngine::update_fps() {
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


void CoreEngine::update_frame_delta() {
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

void CoreEngine::delete_mesh(const std::string& name) {
	mesh_.erase(name);	
}
