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

#include <Jet/Lua/Interpreter.hpp>
#include <Jet/Engine.hpp>
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
#include <Jet/AudioSource.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

using namespace Jet::Lua;
using namespace Jet;

Interpreter::Interpreter() :
    env_(lua_open()) {
        
    luaL_openlibs(env_);
    luabind::open(env_);
    
    // Load Lua bindings for basic value types exported by the engine.
    // These types are enough to perform the majority of operations needed.
    // The rest of the Lua binding is through controllers, and the Node
    // and Object classes.
    luabind::module(env_, "Jet") [
        
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
            
        luabind::class_<Node, NodePtr>("Node")
            .property("parent", &Node::parent)
            .property("position", (void (Node::*)(const Vector&))&Node::position, (const Vector& (Node::*)() const)&Node::position)
            .property("rotation", (void (Node::*)(const Quaternion&))&Node::rotation, (const Quaternion& (Node::*)() const)&Node::rotation)
            .def("node", &Node::node)
            .def("mesh_object", &Node::mesh_object)
            .def("particle_system", &Node::particle_system)
            .def("quad_set", &Node::quad_set)
            .def("quad_chain", &Node::quad_chain)
            .def("light", &Node::light)
            .def("rigid_body", &Node::rigid_body)
            .def("audio_source", &Node::audio_source),
            
        luabind::class_<MeshObject, MeshObjectPtr>("MeshObject")
            .property("parent", &MeshObject::parent)
            .property("material", (void (MeshObject::*)(const std::string&))&MeshObject::material, (Material* (MeshObject::*)() const)&MeshObject::material)
            .property("mesh", (void (MeshObject::*)(const std::string&))&MeshObject::mesh, (Mesh* (MeshObject::*)() const)&MeshObject::mesh)
            .property("cast_shadows", (void (MeshObject::*)(bool))&MeshObject::cast_shadows, (bool (MeshObject::*)() const)&MeshObject::cast_shadows)
            .def("shader_param", (void (MeshObject::*)(const std::string&, const boost::any&))&MeshObject::shader_param)
            .def("shader_param", (const boost::any& (MeshObject::*)(const std::string&))&MeshObject::shader_param),
    
        luabind::class_<ParticleSystem, ParticleSystemPtr>("ParticleSystem")
            .property("parent", &ParticleSystem::parent)
            .property("life", (void (ParticleSystem::*)(const Range&))&ParticleSystem::life, (const Range& (ParticleSystem::*)() const)&ParticleSystem::life)
            .property("width", (void (ParticleSystem::*)(const Range&))&ParticleSystem::width, (const Range& (ParticleSystem::*)() const)&ParticleSystem::width)
            .property("height", (void (ParticleSystem::*)(const Range&))&ParticleSystem::height, (const Range& (ParticleSystem::*)() const)&ParticleSystem::height)
            .property("depth", (void (ParticleSystem::*)(const Range&))&ParticleSystem::depth, (const Range& (ParticleSystem::*)() const)&ParticleSystem::depth)
            .property("speed", (void (ParticleSystem::*)(const Range&))&ParticleSystem::speed, (const Range& (ParticleSystem::*)() const)&ParticleSystem::speed)
            .property("type", (void (ParticleSystem::*)(ParticleSystemType))&ParticleSystem::type, (ParticleSystemType (ParticleSystem::*)() const)&ParticleSystem::type)
            .property("direction", (void (ParticleSystem::*)(const Vector&))&ParticleSystem::direction, (const Vector& (ParticleSystem::*)() const)&ParticleSystem::direction)
            .property("angle", (void (ParticleSystem::*)(const Range&))&ParticleSystem::angle, (const Range& (ParticleSystem::*)() const)&ParticleSystem::angle),
            
        luabind::class_<RigidBody, RigidBodyPtr>("RigidBody")
            .property("parent", &RigidBody::parent)
            .property("linear_velocity", (void (RigidBody::*)(const Vector&))&RigidBody::linear_velocity, (const Vector& (RigidBody::*)() const)&RigidBody::linear_velocity)
            .property("linear_velocity", (void (RigidBody::*)(const Vector&))&RigidBody::linear_velocity, (const Vector& (RigidBody::*)() const)&RigidBody::linear_velocity)
            .property("angular_velocity", (void (RigidBody::*)(const Vector&))&RigidBody::angular_velocity, (const Vector& (RigidBody::*)() const)&RigidBody::angular_velocity)
            .property("apply_force", &RigidBody::apply_force)
            .property("apply_torque", &RigidBody::apply_torque)
            .property("apply_local_force", &RigidBody::apply_local_force)
            .property("apply_local_torque", &RigidBody::apply_local_torque),
            
        luabind::class_<Engine, EnginePtr>("Engine")
            .property("root", &Engine::root)
            
    ];
}

//! Destructor
Interpreter::~Interpreter() {
    lua_close(env_);  
}