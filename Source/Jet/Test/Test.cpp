#include <Jet/Engine.hpp>
#include <Jet/Object.hpp>
#include <Jet/MeshObject.hpp>
#include <Jet/Matrix.hpp>
#include <iostream>
#include <stdexcept>

void main() {
    try {        
        Jet::EnginePtr engine(new Jet::Engine);
        engine->option("shadow_texture_size", 1024);
        engine->option("shadows_enabled", true);
        engine->option("display_width", 1024);
        engine->option("display_height", 768);
		engine->option("window_title", std::string("Extreme Asteroids"));
        engine->option("fullscreen", false);
        
        engine->search_folder("../Textures");
        engine->search_folder("../Meshes");
        engine->search_folder("../Shaders");
        engine->search_folder("../Scripts");
        engine->search_folder("../Sounds");
        engine->search_folder("../Music");
        engine->search_folder("../Blender");
        engine->search_folder("Jet.Core");
		engine->module("Jet.Core");
        engine->module("Jet.OpenGL");
        //engine->module("Jet.Lua");
        //engine->module("Jet.Direct3D9");
        //engine->module("Jet.FMOD");
        //engine->module("Jet.Lua");
        //engine->module("Jet.Python");
        //enigne->module("Jet.XML");
        
        //engine->resource("MetalDiffuse.png");
        engine->resource("Box.obj");
        //engine->resource("Whatever.xml");
        
        
        Jet::NodePtr box_node(engine->root()->node("box"));
        box_node->position(Jet::Vector(5.0f, 0.0f, 0.0f));

        Jet::MeshObjectPtr box(box_node->mesh_object("box"));
        box->mesh("Box.obj");
        box->material("Material_MetalDiffuse.png");
        box->cast_shadows(true);
        
        Jet::NodePtr light_node(engine->root()->node("light"));
        light_node->position(Jet::Vector(10.0f, 0.0f, 10.0f));

		
		Jet::LightPtr light(light_node->light("light"));
		light->type(Jet::POINT_LIGHT);
        light->ambient_color(Jet::Color(0.1f, 0.1f, 0.1f, 1.0f));
        light->diffuse_color(Jet::Color(1.0f, 1.0f, 1.0f, 1.0f));
        light->specular_color(Jet::Color(1.0f, 1.0f, 1.0f, 1.0f));
		
    
        while (engine->running()) {
            engine->tick();
        }
        
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}
