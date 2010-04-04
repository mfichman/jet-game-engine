#include <Jet/Engine.hpp>
#include <Jet/Component.hpp>
#include <iostream>
#include <stdexcept>

void main() {
    try {        
        Jet::EnginePtr engine(new Jet::Engine);
        Jet::ComponentPtr shadows(engine->component("shadows"));
        shadows->type("Config");
        shadows->value("texture_size", 2048);
        shadows->value("enabled", true);
        
        Jet::ComponentPtr window(engine->component("window"));
        window->type("Config");
        window->value("width", 800);
        window->value("height", 600);
        window->value("title", "Extreme Asteroids");
        window->value("fullscreen", 0.0);
        
        engine->folder("../Textures");
        engine->folder("../Meshes");
        engine->folder("../Shaders");
        engine->folder("../Scripts");
        engine->folder("../Sounds");
        engine->folder("../Music");
        engine->folder("../Blender");
        engine->module("Jet.Core");
        engine->module("Jet.OpenGL");
        engine->module("Jet.Lua");
        //engine->module("Jet.Direct3D9");
        //engine->module("Jet.FMOD");
        //engine->module("Jet.Lua");
        //engine->module("Jet.Python");
        //enigne->module("Jet.XML");
        
        //engine->resource("Box.obj");
        //engine->resource("Whatever.xml");
        
        Jet::NodePtr box_node(engine->root()->node("box"));
        Jet::ComponentPtr box = box_node->component("box");
        box->type("MeshObject");
        box->value("renderable", true);
        box->value("mesh", "Box.obj");
        
        Jet::NodePtr light_node(engine->root()->node("light"));
        light_node->position(Jet::Vector(10.0f, 0.0f, 0.0f));
        Jet::ComponentPtr light(light_node->component("light"));
        light->type("PointLight");
        light->value("ambient", Jet::Color(0.0f, 0.0f, 0.0f, 1.0f));
        light->value("diffuse", Jet::Color(1.0f, 1.0f, 1.0f, 1.0f));
        light->value("specular", Jet::Color(0.0f, 0.0f, 1.0f, 1.0f));
    
        while (engine->running()) {
            engine->tick();
        }
        
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}
