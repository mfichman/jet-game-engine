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

#include <Jet/Core/ScriptSystem.hpp>
#include <Jet/Core/ScriptController.hpp>
#include <Jet/Vector.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Range.hpp>
#include <Jet/Color.hpp>
#include <Jet/Node.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/ParticleSystem.hpp>
#include <Jet/QuadSet.hpp>
#include <Jet/QuadChain.hpp>
#include <Jet/Light.hpp>
#include <Jet/RigidBody.hpp>
#include <Jet/Material.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/AudioSource.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Shader.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/operator.hpp>

using namespace Jet;
using namespace std;

Core::ScriptSystem::ScriptSystem(Engine* engine) :
    engine_(engine),
    env_(lua_open()) {
        
    luaL_openlibs(env_);
    luabind::open(env_);
    
    // Load Lua bindings for basic value types exported by the engine.
    // These types are enough to perform the majority of operations needed.
    // The rest of the Lua binding is through controllers, and the Node
    // and Object classes.
    luabind::module(env_) [
        
        luabind::class_<Vector>("Vector")
            .def(luabind::constructor<>())
            .def(luabind::constructor<real_t, real_t, real_t>())
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
            
        luabind::class_<Quaternion>("Quaternion")
            .def(luabind::constructor<>())
            .def(luabind::constructor<real_t, real_t, real_t, real_t>())
            .def(luabind::constructor<const Vector&, real_t>())
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
            .def(luabind::constructor<real_t, real_t, real_t, real_t>())
            .def_readwrite("red", &Color::red)
            .def_readwrite("green", &Color::green)
            .def_readwrite("blue", &Color::blue)
            .def_readwrite("alpha", &Color::alpha)
            .def(luabind::tostring(luabind::const_self)),
            
        luabind::class_<Range>("Range")
            .def(luabind::constructor<>())
            .def(luabind::constructor<real_t, real_t>())
            .def_readwrite("begin", &Range::begin)
            .def_readwrite("end", &Range::end)
            .def(luabind::tostring(luabind::const_self)),
            
        luabind::class_<Jet::Material, Jet::MaterialPtr>("Material")
            .property("ambient_color", (const Color& (Jet::Material::*)() const)&Jet::Material::ambient_color, (void (Jet::Material::*)(const Color&))&Jet::Material::ambient_color)
            .property("diffuse_color", (const Color& (Jet::Material::*)() const)&Jet::Material::diffuse_color, (void (Jet::Material::*)(const Color&))&Jet::Material::diffuse_color)
            .property("specular_color", (const Color& (Jet::Material::*)() const)&Jet::Material::specular_color, (void (Jet::Material::*)(const Color&))&Jet::Material::specular_color)
            .property("diffuse_map", (Jet::Texture* (Jet::Material::*)() const)&Jet::Material::diffuse_map, (void (Jet::Material::*)(const std::string&))&Jet::Material::diffuse_map)
            .property("specular_map", (Jet::Texture* (Jet::Material::*)() const)&Jet::Material::specular_map, (void (Jet::Material::*)(const std::string&))&Jet::Material::specular_map)
            .property("normal_map", (Jet::Texture* (Jet::Material::*)() const)&Jet::Material::normal_map, (void (Jet::Material::*)(const std::string&))&Jet::Material::normal_map)
            .property("shader", (Jet::Shader* (Jet::Material::*)() const)&Jet::Material::shader, (void (Jet::Material::*)(const std::string&))&Jet::Material::shader)
            .property("shininess", (real_t (Jet::Material::*)() const)&Jet::Material::shininess, (void (Jet::Material::*)(real_t))&Jet::Material::shininess)
            .property("shininess", (bool (Jet::Material::*)() const)&Jet::Material::receive_shadows, (void (Jet::Material::*)(bool))&Jet::Material::receive_shadows)
            .property("name", (const std::string& (Jet::Material::*)() const)&Jet::Material::name),
            
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
            .def("look", &Jet::Node::look),
            
        luabind::class_<Jet::MeshObject, Jet::MeshObjectPtr>("MeshObject")
            .property("parent", &Jet::MeshObject::parent)
            .property("material", (Jet::Material* (Jet::MeshObject::*)() const)&Jet::MeshObject::material, (void (Jet::MeshObject::*)(const std::string&))&Jet::MeshObject::material)
            .property("mesh", (Jet::Mesh* (Jet::MeshObject::*)() const)&Jet::MeshObject::mesh, (void (Jet::MeshObject::*)(const std::string&))&Jet::MeshObject::mesh)
            .property("cast_shadows", (bool (Jet::MeshObject::*)() const)&Jet::MeshObject::cast_shadows, (void (Jet::MeshObject::*)(bool))&Jet::MeshObject::cast_shadows)
            .def("shader_param", (void (Jet::MeshObject::*)(const std::string&, const boost::any&))&Jet::MeshObject::shader_param)
            .def("shader_param", (const boost::any& (Jet::MeshObject::*)(const std::string&))&Jet::MeshObject::shader_param),
    
        luabind::class_<Jet::ParticleSystem, Jet::ParticleSystemPtr>("ParticleSystem")
            .property("parent", &Jet::ParticleSystem::parent)
            .property("life", (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::life, (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::life)
            .property("width", (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::width, (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::width)
            .property("height", (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::height, (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::height)
            .property("depth", (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::depth, (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::depth)
            .property("speed", (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::speed, (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::speed)
            .property("type", (void (Jet::ParticleSystem::*)(Jet::ParticleSystemType))&Jet::ParticleSystem::type, (Jet::ParticleSystemType (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::type)
            .property("direction", (void (Jet::ParticleSystem::*)(const Vector&))&Jet::ParticleSystem::direction, (const Vector& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::direction)
            .property("angle", (void (Jet::ParticleSystem::*)(const Range&))&Jet::ParticleSystem::angle, (const Range& (Jet::ParticleSystem::*)() const)&Jet::ParticleSystem::angle),
            
        luabind::class_<Jet::RigidBody, Jet::RigidBodyPtr>("RigidBody")
            .property("parent", &Jet::RigidBody::parent)
            .property("linear_velocity", (Vector (Jet::RigidBody::*)() const)&Jet::RigidBody::linear_velocity, (void (Jet::RigidBody::*)(const Vector&))&Jet::RigidBody::linear_velocity)
            .property("angular_velocity", (Vector (Jet::RigidBody::*)() const)&Jet::RigidBody::angular_velocity, (void (Jet::RigidBody::*)(const Vector&))&Jet::RigidBody::angular_velocity)
            .def("apply_force", &Jet::RigidBody::apply_force)
            .def("apply_torque", &Jet::RigidBody::apply_torque)
            .def("apply_local_force", &Jet::RigidBody::apply_local_force)
            .def("apply_local_torque", &Jet::RigidBody::apply_local_torque)
            .property("mass", (real_t (Jet::RigidBody::*)() const)&Jet::RigidBody::mass, (void (Jet::RigidBody::*)(real_t))&Jet::RigidBody::mass),
            
        luabind::class_<Jet::Engine, Jet::EnginePtr>("Engine")
            .property("root", &Jet::Engine::root)
            
    ];
    
    lua_pushlightuserdata(env_, this);
    lua_pushcclosure(env_, &ScriptSystem::adopt_actor, 1);
    lua_setglobal(env_, "__adopt_actor");
    
    luabind::globals(env_)["engine"] = static_cast<Jet::Engine*>(engine_);
}

//! Destructor
Core::ScriptSystem::~ScriptSystem() {
    lua_close(env_);  
}


void Core::ScriptSystem::on_init() {
    
    string path = engine_->resource_path("Test.lua");
    if (luaL_dofile(env_, path.c_str())) {
        string message(lua_tostring(env_, -1));
        throw runtime_error("Could not load script: " + message);
    }
}

int Core::ScriptSystem::adopt_actor(lua_State* env) {
    using namespace luabind;
    
    ScriptSystem* self = static_cast<ScriptSystem*>(lua_touserdata(env, lua_upvalueindex(1)));
    
    luabind::object ref = object(from_stack(env, 1));
    Jet::Node* node = object_cast<Jet::Node*>(object(from_stack(env, 2)));
    string name = lua_tostring(env, 3);
    
    ObjectPtr obj = new ScriptController(ref, node, name);
    
    return 0;
}