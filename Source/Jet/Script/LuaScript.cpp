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

#include <Jet/Script/LuaTypes.hpp>
#include <Jet/Script/LuaScript.hpp>
#include <Jet/Script/LuaActorState.hpp>
#include <Jet/Script/LuaModule.hpp>
#include <Jet/Script/LuaWidget.hpp>
#include <Jet/Script/LuaTask.hpp>
#include <Jet/Types.hpp>
#include <Jet/Network.hpp>
#include <Jet/Interface/Overlay.hpp>
#include <Jet/Types/Player.hpp>
#include <Jet/Types/Match.hpp>
#include <Jet/Types/Vector.hpp>
#include <Jet/Types/Quaternion.hpp>
#include <Jet/Types/Range.hpp>
#include <Jet/Types/Color.hpp>
#include <Jet/Types/Plane.hpp>
#include <Jet/Types/Box.hpp>
#include <Jet/Types/Point.hpp>
#include <Jet/Types/Matrix.hpp>
#include <Jet/Scene/MeshObject.hpp>
#include <Jet/Scene/Node.hpp>
#include <Jet/Scene/Actor.hpp>
#include <Jet/Scene/FractureObject.hpp>
#include <Jet/Scene/ParticleSystem.hpp>
#include <Jet/Scene/QuadSet.hpp>
#include <Jet/Scene/Camera.hpp>
#include <Jet/Scene/QuadChain.hpp>
#include <Jet/Scene/Light.hpp>
#include <Jet/Scene/RigidBody.hpp>
#include <Jet/Scene/AudioSource.hpp>
#include <Jet/Scene/CollisionSphere.hpp>
#include <Jet/Resources/Material.hpp>
#include <Jet/Resources/Mesh.hpp>
#include <Jet/Resources/Texture.hpp>
#include <Jet/Resources/Shader.hpp>
#include <Jet/Types/Signal.hpp>
#include <Jet/Resources/Font.hpp>
#include <luabind/operator.hpp>
#include <boost/any.hpp>

using namespace Jet;
using namespace std;


namespace luabind {
    
    template <>
    struct default_converter<boost::any> : native_converter_base<boost::any> {
        static int compute_score(lua_State* env, int index) {
            return 0;
        }
        
        boost::any from(lua_State* env, int index) {
            boost::optional<Vector> o1;
            boost::optional<Quaternion> o2;
            switch (lua_type(env, index)) {
                case LUA_TNIL: return boost::any();
                case LUA_TSTRING: return boost::any(string(lua_tostring(env, index)));
                case LUA_TNUMBER: return boost::any((float)lua_tonumber(env, index));
#ifdef WINDOWS
#pragma warning(disable:4800)
#endif
                case LUA_TBOOLEAN: return boost::any((bool)lua_toboolean(env, index));
#ifdef WINDOWS
#pragma warning(default:4800)
#endif
                case LUA_TUSERDATA:
                    o1 = object_cast_nothrow<Vector>(object(from_stack(env, index)));
                    if (o1) {
                        return o1.get();
                    }
                    o2 = object_cast_nothrow<Quaternion>(object(from_stack(env, index)));
                    if (o2) {
                        return o2.get();
                    }

                default: return boost::any();
            }          
        }
        
        void to(lua_State* env, boost::any const& any) {
            if (typeid(float) == any.type()) {
                lua_pushnumber(env, boost::any_cast<float>(any));
            } else if (typeid(string) == any.type()) {
                lua_pushstring(env, boost::any_cast<string>(any).c_str());
            } else if (typeid(bool) == any.type()) {
                lua_pushboolean(env, boost::any_cast<bool>(any));
            } else if (typeid(Vector) == any.type()) {
                object o(env, boost::any_cast<Vector>(any));
                o.push(env);
            } else if (typeid(Quaternion) == any.type()) {
                object o(env, boost::any_cast<Quaternion>(any));
                o.push(env);
            } else {
                lua_pushnil(env);
            }
        }
    };
    
    template <>
    struct default_converter<boost::any const&> : default_converter<boost::any> {
        
    };
}

namespace luabind {
    
    template <>
    struct default_converter<Object*> : native_converter_base<Object*> {
        static int compute_score(lua_State* env, int index) {
            return 0;
        }
        
        Object* from(lua_State* env, int index) {
            return 0;
        }
        
        void to(lua_State* env, Object* any) {
            //lua_pushnil(env);
            lua_pushstring(env, "hello, world!!");
        }
    };
}



LuaScript::LuaScript(CoreEngine* engine) :
    engine_(engine),
    env_(lua_open()) {
	
	try {
		// Open default libraries and Luabind
		luaL_openlibs(env_);
		luabind::open(env_);
	    
		// Load all Luabind bindings
		init_value_type_bindings();
		init_entity_type_bindings();
	    
		// Called when an error occurs
		luabind::set_pcall_callback(&LuaScript::on_error);
        
        lua_getglobal(env_, "Actor");
        lua_pushlightuserdata(env_, this);
        lua_pushcclosure(env_, &LuaScript::adopt_actor_state, 1);
        lua_setfield(env_, -2, "actor_state");
        lua_pop(env_, 1);
	    
		// Add __adopt_widget function
		lua_pushlightuserdata(env_, this);
		lua_pushcclosure(env_, &LuaScript::adopt_widget, 1);
		lua_setglobal(env_, "__adopt_widget");
	    
		// Add __adopt_module function
		lua_pushlightuserdata(env_, this);
		lua_pushcclosure(env_, &LuaScript::adopt_module, 1);
		lua_setglobal(env_, "__adopt_module");
	    
		// Add __adopt_task function
		lua_pushlightuserdata(env_, this);
		lua_pushcclosure(env_, &LuaScript::adopt_task, 1);
		lua_setglobal(env_, "__adopt_task");
	    
		// Set a variable for the engine
		luabind::globals(env_)["engine"] = static_cast<Engine*>(engine_);
	    
		//! Load some utility functions related to the engine
		string engine_path = engine_->resource_path("Engine.lua");
		if (luaL_dofile(env_, engine_path.c_str())) {
			string message(lua_tostring(env_, -1));
			throw runtime_error("Could not load script: " + message);
		}

		//! Load the options file
		string options_path = engine_->resource_path("Options.lua");
		if (luaL_dofile(env_, options_path.c_str())) {
			string message(lua_tostring(env_, -1));
			throw runtime_error("Could not load script: " + message);
		}

	} catch(...) {
		lua_close(env_);
		throw;
	}
    
    engine_->listener(this);
        
}

//! Destructor
LuaScript::~LuaScript() {
	while (!task_.empty()) {
		task_.pop();
	}
    lua_close(env_);
    
}

void LuaScript::on_init() {
    std::cout << "Initializing script system" << std::endl;

    // Add all folders on the search path to the Lua module search path
    string package_path = luabind::object_cast<string>(luabind::globals(env_)["package"]["path"]);
    for (Iterator<const string> i = engine_->search_folders(); i; i++) {
        package_path += ";" + *i + "/?.lua";
    }
    luabind::globals(env_)["package"]["path"] = package_path;
    
    //! Load the main file
    string main_path = engine_->resource_path("Main.lua");
    if (luaL_dofile(env_, main_path.c_str())) {
        string message(lua_tostring(env_, -1));
        throw runtime_error("Could not load script: " + message);
    }
}

void LuaScript::on_update() {
    
    // Execute tasks on top of the heap if the resume time for the task is
    // less than or equal to the current frame time
    float time = engine_->frame_time();
    while (!task_.empty() && task_.top()->resume_time() <= time) {
        LuaTaskPtr task = task_.top();
        task_.pop();
        task->resume();
        if (task->alive()) {
            task_.push(task);
        }
    }
}

int LuaScript::adopt_actor_state(lua_State* env) {
    using namespace luabind;
    LuaScript* self = static_cast<LuaScript*>(lua_touserdata(env, lua_upvalueindex(1)));
	
    Actor* actor = object_cast<Actor*>(object(from_stack(env, 1)));
    string name = lua_tostring(env, 2);
    int ref = lua_ref(env, LUA_REGISTRYINDEX);
    
    LuaActorStatePtr state(new LuaActorState(self->engine_, ref));
    actor->actor_state(name, state.get());

	return 0;
}

int LuaScript::adopt_widget(lua_State* env) {
    using namespace luabind;
    
    LuaScript* self = static_cast<LuaScript*>(lua_touserdata(env, lua_upvalueindex(1)));
	lua_pushvalue(env, 1);
    int ref = lua_ref(env, LUA_REGISTRYINDEX);
    CoreOverlay* overlay = static_cast<CoreOverlay*>(object_cast<Overlay*>(object(from_stack(env, 2))));
    string name = lua_tostring(env, 3);
    
    ObjectPtr obj = new LuaWidget(self->engine_, overlay, ref, name);
    
    return 0;
}

int LuaScript::adopt_module(lua_State* env) {
    using namespace luabind;
    
    LuaScript* self = static_cast<LuaScript*>(lua_touserdata(env, lua_upvalueindex(1)));
    int ref = lua_ref(env, LUA_REGISTRYINDEX);
    self->engine_->module(new LuaModule(self->engine_, ref));
    
    return 0;
}

int LuaScript::adopt_task(lua_State* env) {
    using namespace luabind;
    
    LuaScript* self = static_cast<LuaScript*>(lua_touserdata(env, lua_upvalueindex(1)));
    int ref = lua_ref(env, LUA_REGISTRYINDEX);
    self->task_.push(new LuaTask(self->engine_, ref));
    // Push task on heap
    
    return 0;
}


int LuaScript::on_error(lua_State* env) {
    cout << lua_tostring(env, -1) << endl;
    luabind::object object = luabind::globals(env)["debug"]["traceback"]();
    cout << object << endl;
    return 0;
}


void LuaScript::init_value_type_bindings() {
    // Load Lua bindings for basic value types exported by the engine.
    // These types are enough to perform the majority of operations needed.
    // The rest of the Lua binding is through controllers, and the Node
    // and Object classes.
    luabind::module(env_) [
        
        luabind::class_<Vector>("Vector")
            .def(luabind::constructor<>())
            .def(luabind::constructor<const Vector&>())
            .def(luabind::constructor<float, float, float>())
            .property("length", &Vector::length)
            .property("length2", &Vector::length2)
            .def_readwrite("x", &Vector::x)
            .def_readwrite("y", &Vector::y)
            .def_readwrite("z", &Vector::z)
            .def("lerp", &Vector::lerp)
            .def("distance", &Vector::distance)
            .def("distance2", &Vector::distance2)
            .def("dot", &Vector::dot)
            .def("cross", &Vector::cross)
            .def("__add", &Vector::operator+)
            .def("__sub", (Vector (Vector::*)(const Vector&) const)&Vector::operator-)
            .def("__unm", (Vector (Vector::*)() const)&Vector::operator-)
            .def("__mul", (Vector (Vector::*)(float) const)&Vector::operator*)
            .property("unit", &Vector::unit)
            .property("orthogonal", &Vector::orthogonal)
            .def(luabind::tostring(luabind::const_self)),
            
        luabind::class_<Vertex>("Vertex")
            .def(luabind::constructor<>())
            .def_readwrite("position", &Vertex::position)
            .def_readwrite("normal", &Vertex::normal)
            .def_readwrite("texcoord", &Vertex::texcoord)
            .def_readwrite("tangent", &Vertex::tangent),
            
        luabind::class_<Texcoord>("Texcoord")
            .def(luabind::constructor<float, float>())
            .def(luabind::constructor<>())
            .def_readwrite("u", &Texcoord::u)
            .def_readwrite("v", &Texcoord::v)
            .def(luabind::tostring(luabind::const_self)),
            
        luabind::class_<Quaternion>("Quaternion")
            .def(luabind::constructor<>())
            .def(luabind::constructor<const Quaternion&>())
            .def(luabind::constructor<float, float, float, float>())
            .def(luabind::constructor<const Vector&, float>())
            .def(luabind::constructor<const Vector&, const Vector&, const Vector&>())
            .property("length", &Quaternion::length)
            .property("length2", &Quaternion::length2)
            .def("dot", &Quaternion::dot)
            .def("slerp", &Quaternion::slerp)
            .property("inverse", &Quaternion::inverse)
            .property("unit", &Quaternion::unit)
            .def("__mul", (Vector (Quaternion::*)(const Vector&) const)&Quaternion::operator*)
            .def("__mul", (Quaternion (Quaternion::*)(const Quaternion&) const)&Quaternion::operator*)
            .def(luabind::tostring(luabind::const_self)),
            
        luabind::class_<Color>("Color")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float, float, float>())
            .def_readwrite("red", &Color::red)
            .def_readwrite("green", &Color::green)
            .def_readwrite("blue", &Color::blue)
            .def_readwrite("alpha", &Color::alpha)
            .def(luabind::tostring(luabind::const_self)),
            
        luabind::class_<Range>("Range")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float>())
            .def_readwrite("begin", &Range::begin)
            .def_readwrite("end", &Range::end)
            .def(luabind::tostring(luabind::const_self)),
            
        luabind::class_<Plane>("Plane")
            .def(luabind::constructor<>())
            .def(luabind::constructor<const Vector&, const Vector&>())
            .def(luabind::constructor<const Vector&, const Vector&, const Vector&>())
            .def(luabind::constructor<float, float, float, float>())
            .def_readwrite("a", &Plane::a)
            .def_readwrite("b", &Plane::b)
            .def_readwrite("c", &Plane::c)
            .def_readwrite("d", &Plane::d),
            
        luabind::class_<Point>("Point")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float>())
            .def_readwrite("x", &Point::x)
            .def_readwrite("y", &Point::y),
            
        luabind::class_<Signal>("Signal")
            .def(luabind::constructor<const std::string&>())
            .def(luabind::constructor<const std::string&, const boost::any&, const boost::any&>())
            .def(luabind::constructor<const std::string&, const boost::any&>())
            .def_readonly("name", &Signal::name)
            .def_readonly("first", &Signal::first)
            .def_readonly("second", &Signal::second),

		luabind::class_<Box>("Box")
			.def(luabind::constructor<>())
			.property("width", &Box::width)
			.property("height", &Box::height)
			.property("depth", &Box::depth)
			.property("volume", &Box::volume)
			.property("half_extents", &Box::half_extents)
			.property("origin", &Box::origin)
			.def("point", &Box::point)
			.def_readwrite("min_x", &Box::min_x)
			.def_readwrite("min_y", &Box::min_y)
			.def_readwrite("min_z", &Box::min_x)
			.def_readwrite("max_x", &Box::max_x)
			.def_readwrite("max_y", &Box::max_y)
			.def_readwrite("max_z", &Box::max_z),
            
        luabind::class_<Matrix>("Matrix")
            .def(luabind::constructor<>())
            .def(luabind::constructor<const Quaternion&, const Vector&>())
            .def(luabind::constructor<const Quaternion&>())
            .def(luabind::constructor<const Vector&>())
            .def(luabind::constructor<const Vector&, const Vector&, const Vector&>())
            .property("inverse", &Matrix::inverse)
            .def("__mul", (Matrix (Matrix::*)(const Matrix&) const)&Matrix::operator*)
            .def("__mul", (Vector (Matrix::*)(const Vector&) const)&Matrix::operator*)
            .def("rotate", &Matrix::rotate)
            .property("forward", &Matrix::forward)
            .property("right", &Matrix::right)
            .property("up", &Matrix::up)
            .property("origin", &Matrix::origin)
            .property("rotation", &Matrix::rotation),

		luabind::class_<Player>("Player")
			.def(luabind::constructor<const string&>())
			.def_readonly("name", &Player::name)
			.def_readonly("uuid", &Player::uuid),

		luabind::class_<Match>("Match")
			.def(luabind::constructor<const string&>())
			.def_readonly("name", &Match::name)
			.def_readonly("uuid", &Match::uuid)

    ];
}
    
    
void LuaScript::init_entity_type_bindings() {
    using namespace luabind;

    // Load script bindings for entity types used by the engine
    luabind::module(env_) [
              
              
        luabind::class_<Light, LightPtr>("Light")
            .property("ambient_color", (const Color& (Light::*)() const)&Light::ambient_color, (void (Light::*)(const Color&))&Light::ambient_color)
            .property("diffuse_color", (const Color& (Light::*)() const)&Light::diffuse_color, (void (Light::*)(const Color&))&Light::diffuse_color)
            .property("specular_color", (const Color& (Light::*)() const)&Light::specular_color, (void (Light::*)(const Color&))&Light::specular_color)
            .property("direction", (const Vector& (Light::*)() const)&Light::direction, (void (Light::*)(const Vector&))&Light::direction),
            
         luabind::class_<Material, MaterialPtr>("Material")
            .property("ambient_color", (const Color& (Material::*)() const)&Material::ambient_color, (void (Material::*)(const Color&))&Material::ambient_color)
            .property("diffuse_color", (const Color& (Material::*)() const)&Material::diffuse_color, (void (Material::*)(const Color&))&Material::diffuse_color)
            .property("specular_color", (const Color& (Material::*)() const)&Material::specular_color, (void (Material::*)(const Color&))&Material::specular_color)
            .property("diffuse_map", (Texture* (Material::*)() const)&Material::diffuse_map, (void (Material::*)(const std::string&))&Material::diffuse_map)
            .property("specular_map", (Texture* (Material::*)() const)&Material::specular_map, (void (Material::*)(const std::string&))&Material::specular_map)
            .property("normal_map", (Texture* (Material::*)() const)&Material::normal_map, (void (Material::*)(const std::string&))&Material::normal_map)
            .property("shader", (Shader* (Material::*)() const)&Material::shader, (void (Material::*)(const std::string&))&Material::shader)
            .property("shininess", (float (Material::*)() const)&Material::shininess, (void (Material::*)(float))&Material::shininess)
            .property("receive_shadows", (bool (Material::*)() const)&Material::receive_shadows, (void (Material::*)(bool))&Material::receive_shadows)
            .property("name", (const std::string& (Material::*)() const)&Material::name),
        
        luabind::class_<Camera, CameraPtr>("Camera")
            .property("parent", &Camera::parent)
            .property("active", (bool (Camera::*)() const)&Camera::active, (void (Camera::*)(bool))&Camera::active)
            .property("field_of_view", (float (Camera::*)() const)&Camera::field_of_view, (void (Camera::*)(float))&Camera::field_of_view)
            .property("far_clipping_distance", (float (Camera::*)() const)&Camera::far_clipping_distance, (void (Camera::*)(float))&Camera::far_clipping_distance)
            .property("near_clipping_distance", (float (Camera::*)() const)&Camera::near_clipping_distance, (void (Camera::*)(float))&Camera::near_clipping_distance),
            
        luabind::class_<Node, NodePtr>("Node")
            .property("parent", &Node::parent)
            .property("position", (const Vector& (Node::*)() const)&Node::position, (void (Node::*)(const Vector&))&Node::position)
            .property("rotation", (const Quaternion& (Node::*)() const)&Node::rotation, (void (Node::*)(const Quaternion&))&Node::rotation)
            .property("visible", (bool (Node::*)() const)&Node::visible, (void (Node::*)(bool))&Node::visible)
            .property("matrix", &Node::matrix)
            .property("world_position", &Node::world_position)
            .property("world_rotation", &Node::world_rotation)
            .property("rigid_body", &Node::rigid_body)
            .property("audio_source", &Node::audio_source)
			.property("actor", &Node::actor)
			.def("node", &Node::node)
            .def("mesh_object", &Node::mesh_object)
            .def("particle_system", &Node::particle_system)
            .def("quad_set", &Node::quad_set)
            .def("quad_chain", &Node::quad_chain)
            .def("light", &Node::light)
            .def("camera", &Node::camera)
            .def("fracture_object", &Node::fracture_object)
            .def("collision_sphere", &Node::collision_sphere)
            .def("look", &Node::look)
			.def("destroy", &Node::destroy),
            
        luabind::class_<Actor, ActorPtr>("Actor")
            .property("parent", &Actor::parent)
            .property("state", (const std::string& (Actor::*)() const)&Actor::state, (void (Actor::*)(const std::string&))&Actor::state),
            
        luabind::class_<MeshObject, MeshObjectPtr>("MeshObject")
            .property("parent", &MeshObject::parent)
            .property("material", (Material* (MeshObject::*)() const)&MeshObject::material, (void (MeshObject::*)(const std::string&))&MeshObject::material)
            .property("mesh", (Mesh* (MeshObject::*)() const)&MeshObject::mesh, (void (MeshObject::*)(const std::string&))&MeshObject::mesh)
            .property("cast_shadows", (bool (MeshObject::*)() const)&MeshObject::cast_shadows, (void (MeshObject::*)(bool))&MeshObject::cast_shadows)
            .def("shader_param", (void (MeshObject::*)(const std::string&, const boost::any&))&MeshObject::shader_param)
            .def("shader_param", (const boost::any& (MeshObject::*)(const std::string&))&MeshObject::shader_param),
    
        luabind::class_<FractureObject, FractureObjectPtr>("FractureObject")
            .property("parent", &FractureObject::parent)
            .property("material", (Material* (FractureObject::*)() const)&FractureObject::material, (void (FractureObject::*)(const std::string&))&FractureObject::material)
            .property("mesh", (Mesh* (FractureObject::*)() const)&FractureObject::mesh, (void (FractureObject::*)(const std::string&))&FractureObject::mesh)
            .property("cast_shadows", (bool (FractureObject::*)() const)&FractureObject::cast_shadows, (void (FractureObject::*)(bool))&FractureObject::cast_shadows)
            .property("seal_fractures", (bool (FractureObject::*)() const)&FractureObject::seal_fractures, (void (FractureObject::*)(bool))&FractureObject::seal_fractures)
            .property("fracture_count", (size_t (FractureObject::*)() const)&FractureObject::fracture_count, (void (FractureObject::*)(size_t))&FractureObject::fracture_count)
			.def("fracture", &FractureObject::fracture),
            
        luabind::class_<ParticleSystem, ParticleSystemPtr>("ParticleSystem")
            .property("parent", &ParticleSystem::parent)
            .property("life", (float (ParticleSystem::*)() const)&ParticleSystem::life, (void (ParticleSystem::*)(float))&ParticleSystem::life)
            .property("inherit_velocity", (bool (ParticleSystem::*)() const)&ParticleSystem::inherit_velocity, (void (ParticleSystem::*)(bool))&ParticleSystem::inherit_velocity)
            .property("quota", (size_t (ParticleSystem::*)() const)&ParticleSystem::quota, (void (ParticleSystem::*)(size_t))&ParticleSystem::quota)
			.property("width", (const Range& (ParticleSystem::*)() const)&ParticleSystem::width, (void (ParticleSystem::*)(const Range&))&ParticleSystem::width)
            .property("height", (const Range& (ParticleSystem::*)() const)&ParticleSystem::height, (void (ParticleSystem::*)(const Range&))&ParticleSystem::height)
            .property("depth", (const Range& (ParticleSystem::*)() const)&ParticleSystem::depth, (void (ParticleSystem::*)(const Range&))&ParticleSystem::depth)
            .property("emission_speed", (const Range& (ParticleSystem::*)() const)&ParticleSystem::emission_speed, (void (ParticleSystem::*)(const Range&))&ParticleSystem::emission_speed)
            .property("type", (EmitterType (ParticleSystem::*)() const)&ParticleSystem::type, (void (ParticleSystem::*)(EmitterType))&ParticleSystem::type)
            .property("particle_size", (const Range& (ParticleSystem::*)() const)&ParticleSystem::particle_size, (void (ParticleSystem::*)(const Range&))&ParticleSystem::particle_size)
			.property("particle_life", (const Range& (ParticleSystem::*)() const)&ParticleSystem::particle_life, (void (ParticleSystem::*)(const Range&))&ParticleSystem::particle_life)
			.property("emission_direction", (const Vector& (ParticleSystem::*)() const)&ParticleSystem::emission_direction, (void (ParticleSystem::*)(const Vector&))&ParticleSystem::emission_direction)
            .property("emission_rate", (const Range& (ParticleSystem::*)() const)&ParticleSystem::emission_rate, (void (ParticleSystem::*)(const Range&))&ParticleSystem::emission_rate)
            .property("emission_angle", (const Range& (ParticleSystem::*)() const)&ParticleSystem::emission_angle, (void (ParticleSystem::*)(const Range&))&ParticleSystem::emission_angle)
            .property("particle_growth_rate", (const Range& (ParticleSystem::*)() const)&ParticleSystem::particle_growth_rate, (void (ParticleSystem::*)(const Range&))&ParticleSystem::particle_growth_rate)
			.property("texture", (Texture* (ParticleSystem::*)() const)&ParticleSystem::texture, (void (ParticleSystem::*)(const std::string&))&ParticleSystem::texture)
            .enum_("EmitterType") [ value("ET_BOX", ET_BOX), value("ET_POINT", ET_POINT), value("ET_ELLIPSOID", ET_ELLIPSOID) ],
            
        luabind::class_<AudioSource, AudioSourcePtr>("AudioSource")
            .property("parent", &AudioSource::parent)
            .def("sound", (void (AudioSource::*)(size_t, const std::string&))&AudioSource::sound)
            .def("state", (void (AudioSource::*)(size_t, PlaybackState))&AudioSource::state)
            .def("state", (PlaybackState (AudioSource::*)(size_t) const)&AudioSource::state)
            .enum_("PlaybackState") [ value("PS_PLAY", PS_PLAY), value("PS_STOP", PS_STOP) ],
            
        luabind::class_<RigidBody, RigidBodyPtr>("RigidBody")
            .property("parent", &RigidBody::parent)
            .property("linear_velocity", (Vector (RigidBody::*)() const)&RigidBody::linear_velocity, (void (RigidBody::*)(const Vector&))&RigidBody::linear_velocity)
            .property("angular_velocity", (Vector (RigidBody::*)() const)&RigidBody::angular_velocity, (void (RigidBody::*)(const Vector&))&RigidBody::angular_velocity)
            .def("apply_force", &RigidBody::apply_force)
            .def("apply_torque", &RigidBody::apply_torque)
            .def("apply_local_force", &RigidBody::apply_local_force)
            .def("apply_local_torque", &RigidBody::apply_local_torque)
            .property("mass", (float (RigidBody::*)() const)&RigidBody::mass, (void (RigidBody::*)(float))&RigidBody::mass),
            
        luabind::class_<Engine, EnginePtr>("Engine")
            .property("root", &Engine::root)
            .property("screen", &Engine::screen)
            .property("frame_time", &Engine::frame_time)
            .property("frame_delta", &Engine::frame_delta)           
			.property("network", &Engine::network)
            .property("running", (bool (Engine::*)() const)&Engine::running, (void (Engine::*)(bool))&Engine::running)
			.def("mesh", (Mesh* (Engine::*)(const std::string&))&Engine::mesh)
            .def("material", &Engine::material)
			.def("option", (void (Engine::*)(const std::string&, const boost::any&))&Engine::option)
            .def("option", (const boost::any& (Engine::*)(const std::string&) const)&Engine::option)
            .def("search_folder", &Engine::search_folder),

            
        luabind::class_<Network, NetworkPtr>("Network")
            .property("state", (NetworkState (Network::*)() const)&Network::state, (void (Network::*)(NetworkState))&Network::state)
            .property("current_match", (const Match& (Network::*)() const)&Network::current_match, (void (Network::*)(const Match&))&Network::current_match)
            .property("current_player", (const Player& (Network::*)() const)&Network::current_player, (void (Network::*)(const Player&))&Network::current_player)
            .property("match_count", &Network::match_count)
            .property("player_count", &Network::player_count)
            .def("match", &Network::match)
            .def("player", &Network::player)
            .enum_("NetworkState") [ value("NS_JOIN", NS_JOIN), value("NS_HOST", NS_HOST), value("NS_DISCOVER", NS_DISCOVER), value("NS_DISABLED", NS_DISABLED) ],

            
        luabind::class_<Mesh, MeshPtr>("Mesh")
            .def("vertex", (void (Mesh::*)(size_t, const Vertex&))&Mesh::vertex)
            .def("index", (void (Mesh::*)(size_t, uint32_t))&Mesh::index),
              
        luabind::class_<CollisionSphere, CollisionSpherePtr>("CollisionSphere")
            .property("parent", &CollisionSphere::parent)
            .property("radius", (float (CollisionSphere::*)() const)&CollisionSphere::radius, (void (CollisionSphere::*)(float))&CollisionSphere::radius),

                    
        luabind::class_<Overlay, OverlayPtr>("Overlay")
            .def("overlay", &Overlay::overlay)
            .property("x", (float (Overlay::*)() const)&Overlay::x, (void (Overlay::*)(float))&Overlay::x)
            .property("y", (float (Overlay::*)() const)&Overlay::y, (void (Overlay::*)(float))&Overlay::y)
            .property("width", (float (Overlay::*)() const)&Overlay::width, (void (Overlay::*)(float))&Overlay::width)
            .property("height", (float (Overlay::*)() const)&Overlay::height, (void (Overlay::*)(float))&Overlay::height)
            .property("layout_mode", (LayoutMode (Overlay::*)() const)&Overlay::layout_mode, (void (Overlay::*)(LayoutMode))&Overlay::layout_mode)
            .property("visible", (bool (Overlay::*)() const)&Overlay::visible, (void (Overlay::*)(bool))&Overlay::visible)
            .property("focusable", (bool (Overlay::*)() const)&Overlay::focusable, (void (Overlay::*)(bool))&Overlay::focusable)
            .property("vertical_alignment", (Alignment (Overlay::*)() const)&Overlay::vertical_alignment, (void (Overlay::*)(Alignment))&Overlay::vertical_alignment)
            .property("horizontal_alignment", (Alignment (Overlay::*)() const)&Overlay::horizontal_alignment, (void (Overlay::*)(Alignment))&Overlay::horizontal_alignment)
            .property("text", (const std::string& (Overlay::*)() const)&Overlay::text, (void (Overlay::*)(const std::string&))&Overlay::text)
            .property("text_color", (const Color& (Overlay::*)() const)&Overlay::text_color, (void (Overlay::*)(const Color&))&Overlay::text_color)
            .property("font", (Font* (Overlay::*)() const)&Overlay::font, (void (Overlay::*)(const std::string&))&Overlay::font)
            .property("background", (Texture* (Overlay::*)() const)&Overlay::background, (void (Overlay::*)(const std::string&))&Overlay::background)
            .property("mouse_over", &Overlay::mouse_over)
            .enum_("Alignment") [ value("AL_LEFT", AL_LEFT), value("AL_RIGHT", AL_RIGHT), value("AL_CENTER", AL_CENTER), value("AL_TOP", AL_TOP), value("AL_BOTTOM", AL_BOTTOM) ]
            .enum_("LayoutMode") [ value("LM_RELATIVE", LM_RELATIVE), value("LM_ABSOLUTE", LM_ABSOLUTE) ]
            
    ];
}

bool LuaScript::Compare::operator()(LuaTaskPtr t0, LuaTaskPtr t1) {
    return t0->resume_time() > t1->resume_time();
}
