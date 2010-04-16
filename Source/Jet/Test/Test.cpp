#include <Jet/Engine.hpp>
#include <Jet/Object.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/Camera.hpp>
#include <Jet/Mesh.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Node.hpp>
#include <Jet/Light.hpp>
#include <Jet/Material.hpp>
#include <Jet/Quaternion.hpp>
#include <Jet/Vertex.hpp>
#include <iostream>
#include <stdexcept>
#include <cmath>

void main() {
    try {        
        Jet::EnginePtr engine(Jet::Engine::create());
        engine->option("shadow_texture_size", 1024);
        engine->option("shadows_enabled", true);
        engine->option("display_width", 1200);
        engine->option("display_height", 800);
		engine->option("window_title", std::string("Extreme Asteroids"));
        engine->option("fullscreen", false);
        
        engine->search_folder("../Textures");
        engine->search_folder("../Meshes");
        engine->search_folder("../Shaders");
        engine->search_folder("../Scripts");
        engine->search_folder("../Sounds");
        engine->search_folder("../Music");
        engine->search_folder("../Blender");    

		/*********************************************/
		Jet::MeshPtr plane_mesh(engine->mesh("plane"));
		plane_mesh->sync_mode(Jet::DYNAMIC_SYNC);
		
		Jet::MaterialPtr plane_material(engine->material("plane_material"));
		plane_material->diffuse_color(Jet::Color(1.0f, 1.0f, 1.0f, 1.0f));
		plane_material->diffuse_map("MetalDiffuse.png");

		Jet::MeshObjectPtr plane(engine->root()->mesh_object("plane"));
		plane->mesh("plane");
		plane->material(plane_material.get());
		
		Jet::Vertex v;
		v.normal = Jet::Vector(1.0f, 0.0f, 0.0f);
		v.tangent = Jet::Vector(0.0f, 0.0f, 1.0f);
		v.position = Jet::Vector(-3.0f, 10.0f, -10.0f);
		v.texcoord = Jet::Texcoord(1.0f, 0.0f);
		plane_mesh->vertex(0, v);
		
		v.normal = Jet::Vector(1.0f, 0.0f, 0.0f);
		v.tangent = Jet::Vector(0.0f, 1.0f, 0.0f);
		v.position = Jet::Vector(-3.0f, 10.0f, 10.0f);
		v.texcoord = Jet::Texcoord(1.0f, 1.0f);
		plane_mesh->vertex(1, v);
		
		v.normal = Jet::Vector(1.0f, 0.0f, 0.0f);
		v.tangent = Jet::Vector(0.0f, 0.0f, 1.0f);
		v.position = Jet::Vector(-3.0f, -10.0f, 10.0f);
		v.texcoord = Jet::Texcoord(0.0f, 1.0f);
		plane_mesh->vertex(2, v);
		
		v.normal = Jet::Vector(1.0f, 0.0f, 0.0f);
		v.tangent = Jet::Vector(0.0f, 1.0f, 0.0f);
		v.position = Jet::Vector(-3.0f, -10.0f, -10.0f);
		v.texcoord = Jet::Texcoord(0.0f, 0.0f);
		plane_mesh->vertex(3, v);

		plane_mesh->index(0, 0);
		plane_mesh->index(1, 2);
		plane_mesh->index(2, 3);

		plane_mesh->index(3, 0);
		plane_mesh->index(4, 1);
		plane_mesh->index(5, 2);
        
		/*********************************************/
        
        Jet::NodePtr light_node(engine->root()->node("light"));
        light_node->position(Jet::Vector(20.0f, 0.0f, 0.0f));

		Jet::LightPtr light(light_node->light("light"));
		light->type(Jet::DIRECTIONAL_LIGHT);
        light->ambient_color(Jet::Color(0.3f, 0.3f, 0.3f, 1.0f));
        light->diffuse_color(Jet::Color(1.0f, 1.0f, 1.0f, 1.0f));
        light->specular_color(Jet::Color(1.0f, 1.0f, 1.0f, 1.0f));
		light->direction(Jet::Vector(1.0f, 0.0f, 0.0f));
		
		Jet::NodePtr camera_node(engine->root()->node("camera"));
		
		Jet::CameraPtr camera(camera_node->camera());
		camera->active(true);

		float pos = 0.0f;
        while (engine->running()) {
			engine->tick();
			pos += 0.001f;
			camera_node->position(Jet::Vector(15.0f*sinf(pos), 0.0f, 15.0f*cosf(pos)));
			camera_node->look(Jet::Vector(0.0f, 0.0f, 0.0f), Jet::Vector(0.0f, 1.0f, 0.0f));
        }
        
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}
