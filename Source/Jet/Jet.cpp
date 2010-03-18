#include <Jet/Jet.hpp>
#include <Jet/Material.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/ParticleSystem.hpp>
#include <Jet/Quad.hpp>
#include <Jet/RigidBody.hpp>
#include <Jet/SceneNode.hpp>
#include <Jet/AudioSource.hpp>
#include <Jet/Engine.hpp>

#include <iostream>

#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#include <lua/lua.hpp>

#include <stdexcept>
using namespace Jet;
using namespace std;

void create_bindings(lua_State* env);


Engine* engine() {
    static Engine engine;
    return &engine;
}

void main() {
    lua_State* env = lua_open();
    try {
        luaL_openlibs(env);
        luabind::open(env);
        create_bindings(env);

        luabind::module(env) [
            luabind::def("engine", &engine)
        ];
        
        if (luaL_dostring(env, 
            "engine().root:add_scene_node(\"bob\", \"bob\")\n"
            "print(engine().root:scene_node(\"bob\").full_name)\n"
            "print(engine().root:mesh_object(\"ship\").mesh)\n"
            )) {
            cout << lua_tostring(env, -1) << endl;
        }

    } catch (exception& ex) {
        cout << ex.what() << endl;
    }

    lua_close(env);    
}

void create_bindings(lua_State* env) {
   luabind::module(env) [
        luabind::class_<Engine>("Engine")
            .property("root", &Engine::get_root)
    ];

    luabind::module(env) [
        luabind::class_<Vector>("Vector")
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
    ];

    luabind::module(env) [
        luabind::class_<Quaternion>("Quaternion")
            .def(luabind::constructor<real_t, real_t, real_t, real_t>())
            .def(luabind::constructor<const Vector&, real_t>())
            .def(luabind::constructor<const Vector&, const Vector&, const Vector&>())
            .property("length", &Quaternion::length)
            .property("length2", &Quaternion::length2)
            .def("dot", &Quaternion::dot)
            .def("slerp", &Quaternion::slerp)
            .property("inverse", &Quaternion::inverse)
            .property("unit", &Quaternion::unit)
    ];
    /*

    luabind::module(env) [
        luabind::class_<SceneComponent>("SceneComponent")
            .property("name", &SceneComponent::get_name)
            .property("parent", &SceneComponent::get_parent)
            .property("full_name", &SceneComponent::get_full_name)
    ];

    luabind::module(env) [
        luabind::class_<AudioSource, SceneComponent>("AudioSource")
            .property("sound", &AudioSource::get_sound, &AudioSource::set_sound)
            .property("gain", &AudioSource::get_gain, &AudioSource::set_gain)
            .property("looping_enabled", &AudioSource::is_looping_enabled, &AudioSource::set_looping_enabled)
            .property("playing", &AudioSource::is_playing, &AudioSource::set_playing)
    ];

    luabind::module(env) [
        luabind::class_<MeshObject, SceneComponent>("MeshObject")
            .property("material", &MeshObject::get_material, &MeshObject::set_material)
            .property("mesh", &MeshObject::get_mesh, &MeshObject::set_mesh)
    ];

    luabind::module(env) [
        luabind::class_<Quad, SceneComponent>("Quad")
            .property("material", &Quad::get_material, &Quad::set_material)
            .property("width", &Quad::get_width, &Quad::set_width)
            .property("height", &Quad::get_height, &Quad::set_height)
    ];

    luabind::module(env) [
        luabind::class_<RigidBody, SceneComponent>("RigidBody")
            .property("linear_velocity", &RigidBody::get_linear_velocity, &RigidBody::set_linear_velocity)
            .property("angular_velocity", &RigidBody::get_angular_velocity, &RigidBody::set_angular_velocity)
            .property("position", &RigidBody::get_position, &RigidBody::set_position)
            .property("rotation", &RigidBody::get_rotation, &RigidBody::set_rotation)
            .def("apply_force", &RigidBody::apply_force)
            .def("apply_torque", &RigidBody::apply_torque)
            .def("apply_local_force", &RigidBody::apply_local_force)
            .def("apply_local_torque", &RigidBody::apply_local_torque)
    ];

    luabind::module(env) [
        luabind::class_<ParticleSystem, SceneComponent>("ParticleSystem")
            .property("material", &ParticleSystem::get_material, &ParticleSystem::set_material)
            .property("quota", &ParticleSystem::get_quota, &ParticleSystem::set_quota)
            .property("life", &ParticleSystem::get_life, &ParticleSystem::set_life)
    ];


    luabind::module(env) [
        luabind::class_<SceneNode, SceneComponent>("SceneNode")

            .def("particle_system", &SceneNode::get_particle_system)
            .def("mesh_object", &SceneNode::get_mesh_object)
            .def("quad", &SceneNode::get_quad)
            .def("audio_source", &SceneNode::get_audio_source)
            .def("rigid_body", &SceneNode::get_rigid_body)
            //.def("get_network_monitor", &SceneNode::get_network_monitor)
            .def("scene_node", &SceneNode::get_scene_node)
            //.def("controller", &SceneNode::get_controller)
            .def_readonly("components", &SceneNode::get_components, luabind::return_stl_iterator)
            .def_readonly("scene_nodes", &SceneNode::get_scene_nodes, luabind::return_stl_iterator)
            //.def_readonly("controllers", &SceneNode::get_controllers, luabind::return_stl_iterator)
            .def("add_scene_node", &SceneNode::add_scene_node)
            //.def("add_controller", &SceneNode::add_controller)
            .def("remove_component", &SceneNode::remove_component)
            .def("remove_scene_node", &SceneNode::remove_scene_node)            
            //.def("remove_controller", &SceneNode::remove_controller)
            .property("position", &SceneNode::get_position, &SceneNode::set_position)
            .property("rotation", &SceneNode::get_rotation, &SceneNode::set_position)
    ];*/
}
