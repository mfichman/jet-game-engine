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
#include <Jet/Core/Types.hpp>
#include <Jet/Core/ScriptSystem.hpp>
#include <Jet/Core/ScriptController.hpp>
#include <Jet/Core/ScriptModule.hpp>
#include <Jet/Overlay.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Range.hpp>
#include <Jet/Color.hpp>
#include <Jet/Plane.hpp>
#include <Jet/Node.hpp>
#include <Jet/Box.hpp>
#include <Jet/Point.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/FractureObject.hpp>
#include <Jet/ParticleSystem.hpp>
#include <Jet/QuadSet.hpp>
#include <Jet/Camera.hpp>
#include <Jet/QuadChain.hpp>
#include <Jet/Light.hpp>
#include <Jet/RigidBody.hpp>
#include <Jet/Material.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/AudioSource.hpp>
#include <Jet/Texture.hpp>
#include <Jet/FractalPlanet.hpp>
#include <Jet/Shader.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
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
    struct default_converter<Jet::Object*> : native_converter_base<Jet::Object*> {
        static int compute_score(lua_State* env, int index) {
            return 0;
        }
        
        Jet::Object* from(lua_State* env, int index) {
            return 0;
        }
        
        void to(lua_State* env, Jet::Object* any) {
            //lua_pushnil(env);
            lua_pushstring(env, "hello, world!!");
        }
    };
}



Core::ScriptSystem::ScriptSystem(Engine* engine) :
    engine_(engine),
    env_(lua_open()) {
        
    engine_->listener(this);
        
    // Open default libraries and Luabind
    luaL_openlibs(env_);
    luabind::open(env_);
    
    // Load all Luabind bindings
    init_value_type_bindings();
    init_entity_type_bindings();
    
    // Add __adopt_actor function
    lua_pushlightuserdata(env_, this);
    lua_pushcclosure(env_, &ScriptSystem::adopt_actor, 1);
    lua_setglobal(env_, "__adopt_actor");
    
    // Add __adopt_module function
    lua_pushlightuserdata(env_, this);
    lua_pushcclosure(env_, &ScriptSystem::adopt_module, 1);
    lua_setglobal(env_, "__adopt_module");
    
    // Set a variable for the engine
    luabind::globals(env_)["engine"] = static_cast<Jet::Engine*>(engine_);
    
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
}

//! Destructor
Core::ScriptSystem::~ScriptSystem() {
    lua_close(env_);
    
}

void Core::ScriptSystem::on_init() {
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

int Core::ScriptSystem::adopt_actor(lua_State* env) {
    using namespace luabind;
    
    luabind::object ref = object(from_stack(env, 1));
    Jet::Node* node = object_cast<Jet::Node*>(object(from_stack(env, 2)));
    string name = lua_tostring(env, 3);
    
    ObjectPtr obj = new ScriptController(ref, node, name);
    
    return 0;
}

int Core::ScriptSystem::adopt_module(lua_State* env) {
    using namespace luabind;
    
    ScriptSystem* self = static_cast<ScriptSystem*>(lua_touserdata(env, lua_upvalueindex(1)));
    luabind::object ref = object(from_stack(env, -1));
    self->engine_->module(new ScriptModule(ref));
    
    return 0;
}

void Core::ScriptSystem::init_value_type_bindings() {
    // Load Lua bindings for basic value types exported by the engine.
    // These types are enough to perform the majority of operations needed.
    // The rest of the Lua binding is through controllers, and the Node
    // and Object classes.
    luabind::module(env_) [
        
        luabind::class_<Vector>("Vector")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float, float>())
            .property("length", &Vector::length)
            .property("length2", &Vector::length2)
            .def_readwrite("x", &Vector::x)
            .def_readwrite("y", &Vector::y)
            .def_readwrite("z", &Vector::z)
            .def("distance", &Vector::distance)
            .def("distance2", &Vector::distance2)
            .def("dot", &Vector::dot)
            .def("cross", &Vector::cross)
            .def("__add", &Vector::operator+)
            .def("__sub", (Vector (Vector::*)(const Vector&) const)&Vector::operator-)
            .def("__unm", (Vector (Vector::*)() const)&Vector::operator-)
            .def("__mul", (Vector (Vector::*)(float) const)&Vector::operator*)
            .property("unit", &Vector::unit)
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
            .def(luabind::constructor<float, float, float, float>())
            .def(luabind::constructor<const Vector&, float>())
            .def(luabind::constructor<const Vector&, const Vector&, const Vector&>())
            .property("length", &Quaternion::length)
            .property("length2", &Quaternion::length2)
            .def("dot", &Quaternion::dot)
            .def("slerp", &Quaternion::slerp)
            .property("inverse", &Quaternion::inverse)
            .property("unit", &Quaternion::unit)
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
			.def_readwrite("max_z", &Box::max_z)
    ];
}
    
    
void Core::ScriptSystem::init_entity_type_bindings() {
    using namespace luabind;

    // Load script bindings for entity types used by the engine
    luabind::module(env_) [     
        luabind::class_<Jet::Light, Jet::LightPtr>("Light")
            .property("ambient_color", (const Color& (Jet::Light::*)() const)&Jet::Light::ambient_color, (void (Jet::Light::*)(const Color&))&Jet::Light::ambient_color)
            .property("diffuse_color", (const Color& (Jet::Light::*)() const)&Jet::Light::diffuse_color, (void (Jet::Light::*)(const Color&))&Jet::Light::diffuse_color)
            .property("specular_color", (const Color& (Jet::Light::*)() const)&Jet::Light::specular_color, (void (Jet::Light::*)(const Color&))&Jet::Light::specular_color)
            .property("direction", (const Vector& (Jet::Light::*)() const)&Jet::Light::direction, (void (Jet::Light::*)(const Vector&))&Jet::Light::direction),
            
         luabind::class_<Jet::Material, Jet::MaterialPtr>("Material")
            .property("ambient_color", (const Color& (Jet::Material::*)() const)&Jet::Material::ambient_color, (void (Jet::Material::*)(const Color&))&Jet::Material::ambient_color)
            .property("diffuse_color", (const Color& (Jet::Material::*)() const)&Jet::Material::diffuse_color, (void (Jet::Material::*)(const Color&))&Jet::Material::diffuse_color)
            .property("specular_color", (const Color& (Jet::Material::*)() const)&Jet::Material::specular_color, (void (Jet::Material::*)(const Color&))&Jet::Material::specular_color)
            .property("diffuse_map", (Jet::Texture* (Jet::Material::*)() const)&Jet::Material::diffuse_map, (void (Jet::Material::*)(const std::string&))&Jet::Material::diffuse_map)
            .property("specular_map", (Jet::Texture* (Jet::Material::*)() const)&Jet::Material::specular_map, (void (Jet::Material::*)(const std::string&))&Jet::Material::specular_map)
            .property("normal_map", (Jet::Texture* (Jet::Material::*)() const)&Jet::Material::normal_map, (void (Jet::Material::*)(const std::string&))&Jet::Material::normal_map)
            .property("shader", (Jet::Shader* (Jet::Material::*)() const)&Jet::Material::shader, (void (Jet::Material::*)(const std::string&))&Jet::Material::shader)
            .property("shininess", (float (Jet::Material::*)() const)&Jet::Material::shininess, (void (Jet::Material::*)(float))&Jet::Material::shininess)
            .property("receive_shadows", (bool (Jet::Material::*)() const)&Jet::Material::receive_shadows, (void (Jet::Material::*)(bool))&Jet::Material::receive_shadows)
            .property("name", (const std::string& (Jet::Material::*)() const)&Jet::Material::name),
        
        luabind::class_<Jet::Camera, Jet::CameraPtr>("Camera")
            .property("parent", &Jet::Camera::parent)
            .property("active", (bool (Jet::Camera::*)() const)&Jet::Camera::active, (void (Jet::Camera::*)(bool))&Jet::Camera::active)
            .property("field_of_view", (float (Jet::Camera::*)() const)&Jet::Camera::field_of_view, (void (Jet::Camera::*)(float))&Jet::Camera::field_of_view)
            .property("far_clipping_distance", (float (Jet::Camera::*)() const)&Jet::Camera::far_clipping_distance, (void (Jet::Camera::*)(float))&Jet::Camera::far_clipping_distance)
            .property("near_clipping_distance", (float (Jet::Camera::*)() const)&Jet::Camera::near_clipping_distance, (void (Jet::Camera::*)(float))&Jet::Camera::near_clipping_distance),
            
        luabind::class_<Jet::Node, Jet::NodePtr>("Node")
            .property("parent", &Jet::Node::parent)
            .property("position", (const Vector& (Jet::Node::*)() const)&Jet::Node::position, (void (Jet::Node::*)(const Vector&))&Jet::Node::position)
            .property("rotation", (const Quaternion& (Jet::Node::*)() const)&Jet::Node::rotation, (void (Jet::Node::*)(const Quaternion&))&Jet::Node::rotation)
            .def("node", &Jet::Node::node)
            .def("mesh_object", &Jet::Node::mesh_object)
            .def("particle_system", &Jet::Node::particle_system)
            .def("quad_set", &Jet::Node::quad_set)
            .def("quad_chain", &Jet::Node::quad_chain)
            .def("light", &Jet::Node::light)
            .def("rigid_body", &Jet::Node::rigid_body)
            .def("audio_source", &Jet::Node::audio_source)
            .def("camera", &Jet::Node::camera)
            .def("fracture_object", &Jet::Node::fracture_object)
            .def("fractal_planet", &Jet::Node::fractal_planet)
            .def("look", &Jet::Node::look)
			.def("destroy", &Jet::Node::destroy),
            
        luabind::class_<Jet::MeshObject, Jet::MeshObjectPtr>("MeshObject")
            .property("parent", &Jet::MeshObject::parent)
            .property("material", (Jet::Material* (Jet::MeshObject::*)() const)&Jet::MeshObject::material, (void (Jet::MeshObject::*)(const std::string&))&Jet::MeshObject::material)
            .property("mesh", (Jet::Mesh* (Jet::MeshObject::*)() const)&Jet::MeshObject::mesh, (void (Jet::MeshObject::*)(const std::string&))&Jet::MeshObject::mesh)
            .property("cast_shadows", (bool (Jet::MeshObject::*)() const)&Jet::MeshObject::cast_shadows, (void (Jet::MeshObject::*)(bool))&Jet::MeshObject::cast_shadows)
            .def("shader_param", (void (Jet::MeshObject::*)(const std::string&, const boost::any&))&Jet::MeshObject::shader_param)
            .def("shader_param", (const boost::any& (Jet::MeshObject::*)(const std::string&))&Jet::MeshObject::shader_param),
    
        luabind::class_<Jet::FractureObject, Jet::FractureObjectPtr>("FractureObject")
            .property("parent", &Jet::FractureObject::parent)
            .property("material", (Jet::Material* (Jet::FractureObject::*)() const)&Jet::FractureObject::material, (void (Jet::FractureObject::*)(const std::string&))&Jet::FractureObject::material)
            .property("mesh", (Jet::Mesh* (Jet::FractureObject::*)() const)&Jet::FractureObject::mesh, (void (Jet::FractureObject::*)(const std::string&))&Jet::FractureObject::mesh)
            .property("cast_shadows", (bool (Jet::FractureObject::*)() const)&Jet::FractureObject::cast_shadows, (void (Jet::FractureObject::*)(bool))&Jet::FractureObject::cast_shadows)
            .property("seal_fractures", (bool (Jet::FractureObject::*)() const)&Jet::FractureObject::seal_fractures, (void (Jet::FractureObject::*)(bool))&Jet::FractureObject::seal_fractures)
            .property("fracture_count", (size_t (Jet::FractureObject::*)() const)&Jet::FractureObject::fracture_count, (void (Jet::FractureObject::*)(size_t))&Jet::FractureObject::fracture_count)
			.def("fracture", &Jet::FractureObject::fracture),
            
        luabind::class_<Jet::ParticleSystem, Jet::ParticleSystemPtr>("ParticleSystem")
            .property("parent", &Jet::ParticleSystem::parent)
            .property("life", (float (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::life, (void (Jet::ParticleSystem::*)(float))&Jet::ParticleSystem::life)
            .property("quota", (size_t (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::quota, (void (Jet::ParticleSystem::*)(size_t))&Jet::ParticleSystem::quota)
			.property("width", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::width, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::width)
            .property("height", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::height, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::height)
            .property("depth", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::depth, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::depth)
            .property("emission_speed", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::emission_speed, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::emission_speed)
            .property("type", (Jet::EmitterType (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::type, (void (Jet::ParticleSystem::*)(Jet::EmitterType))&Jet::ParticleSystem::type)
            .property("particle_size", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::particle_size, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::particle_size)
			.property("particle_life", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::particle_life, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::particle_life)
			.property("emission_direction", (const Vector& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::emission_direction, (void (Jet::ParticleSystem::*)(const Vector&))&Jet::ParticleSystem::emission_direction)
            .property("emission_rate", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::emission_rate, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::emission_rate)
            .property("emission_angle", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::emission_angle, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::emission_angle)
            .property("particle_growth_rate", (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::particle_growth_rate, (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::particle_growth_rate)
			.property("texture", (Jet::Texture* (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::texture, (void (Jet::ParticleSystem::*)(const std::string&))&Jet::ParticleSystem::texture)
            .enum_("EmitterType") [ value("BOX_EMITTER", BOX_EMITTER), value("POINT_EMITTER", POINT_EMITTER), value("ELLIPSOID_EMITTER", ELLIPSOID_EMITTER) ],
            
        luabind::class_<Jet::RigidBody, Jet::RigidBodyPtr>("RigidBody")
            .property("parent", &Jet::RigidBody::parent)
            .property("linear_velocity", (Vector (Jet::RigidBody::*)() const)&Jet::RigidBody::linear_velocity, (void (Jet::RigidBody::*)(const Vector&))&Jet::RigidBody::linear_velocity)
            .property("angular_velocity", (Vector (Jet::RigidBody::*)() const)&Jet::RigidBody::angular_velocity, (void (Jet::RigidBody::*)(const Vector&))&Jet::RigidBody::angular_velocity)
            .def("apply_force", &Jet::RigidBody::apply_force)
            .def("apply_torque", &Jet::RigidBody::apply_torque)
            .def("apply_local_force", &Jet::RigidBody::apply_local_force)
            .def("apply_local_torque", &Jet::RigidBody::apply_local_torque)
            .property("mass", (float (Jet::RigidBody::*)() const)&Jet::RigidBody::mass, (void (Jet::RigidBody::*)(float))&Jet::RigidBody::mass),
            
        luabind::class_<Jet::Engine, Jet::EnginePtr>("Engine")
            .property("root", &Jet::Engine::root)
            .property("overlay", &Jet::Engine::overlay)
            .def("option", (void (Jet::Engine::*)(const std::string&, const boost::any&))&Jet::Engine::option)
            .def("option", (const boost::any& (Jet::Engine::*)(const std::string&) const)&Jet::Engine::option)
            .def("search_folder", &Jet::Engine::search_folder)
			.def("mesh", (Jet::Mesh* (Jet::Engine::*)(const std::string&))&Jet::Engine::mesh)
            .def("material", &Jet::Engine::material)
            .property("running", (bool (Jet::Engine::*)() const)&Jet::Engine::running, (void (Jet::Engine::*)(bool))&Jet::Engine::running),
            
        luabind::class_<Jet::Mesh, Jet::MeshPtr>("Mesh")
            .def("vertex", (void (Jet::Mesh::*)(size_t, const Vertex&))&Jet::Mesh::vertex)
            .def("index", (void (Jet::Mesh::*)(size_t, uint32_t))&Jet::Mesh::index),
            
        luabind::class_<Jet::FractalPlanet, Jet::FractalPlanetPtr>("FractalPlanet"),
            
        luabind::class_<Jet::Overlay, Jet::OverlayPtr>("Overlay")
            .def("overlay", &Jet::Overlay::overlay)
            .property("x", (float (Jet::Overlay::*)() const)&Jet::Overlay::x, (void (Jet::Overlay::*)(float))&Jet::Overlay::x)
            .property("y", (float (Jet::Overlay::*)() const)&Jet::Overlay::y, (void (Jet::Overlay::*)(float))&Jet::Overlay::y)
            .property("width", (float (Jet::Overlay::*)() const)&Jet::Overlay::width, (void (Jet::Overlay::*)(float))&Jet::Overlay::width)
            .property("height", (float (Jet::Overlay::*)() const)&Jet::Overlay::height, (void (Jet::Overlay::*)(float))&Jet::Overlay::height)
            .property("layout_mode", (LayoutMode (Jet::Overlay::*)() const)&Jet::Overlay::layout_mode, (void (Jet::Overlay::*)(LayoutMode))&Jet::Overlay::layout_mode)
            .property("vertical_alignment", (Alignment (Jet::Overlay::*)() const)&Jet::Overlay::vertical_alignment, (void (Jet::Overlay::*)(Alignment))&Jet::Overlay::vertical_alignment)
            .property("horizontal_alignment", (Alignment (Jet::Overlay::*)() const)&Jet::Overlay::horizontal_alignment, (void (Jet::Overlay::*)(Alignment))&Jet::Overlay::horizontal_alignment)
            .property("text", (const std::string& (Jet::Overlay::*)() const)&Jet::Overlay::text, (void (Jet::Overlay::*)(const std::string&))&Jet::Overlay::text)
            //.property("font", (Jet::Font* (Jet::Overlay::*)() const)&Jet::Overlay::font, (void (Jet::Overlay::*)(const std::string&))&Jet::Overlay::font)
            .property("background", (Jet::Texture* (Jet::Overlay::*)() const)&Jet::Overlay::background, (void (Jet::Overlay::*)(const std::string&))&Jet::Overlay::background)
            .enum_("Alignment") [ value("LEFT", LEFT), value("RIGHT", RIGHT), value("CENTER", CENTER), value("TOP", TOP), value("BOTTOM", BOTTOM) ]
            .enum_("LayoutMode") [ value("RELATIVE_LAYOUT", RELATIVE_LAYOUT), value("ABSOLUTE_LAYOUT", ABSOLUTE_LAYOUT) ]
            
    ];
}
