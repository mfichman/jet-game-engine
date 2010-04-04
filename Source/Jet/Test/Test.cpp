#include <Jet/Engine.hpp>
#include <Jet/Component.hpp>
#include <iostream>
#include <stdexcept>

void main() {
    try {
        Jet::ComponentPtr shadow(new Jet::Component("Config"));
        shadow->value("texture_size", 2048);
        shadow->value("enabled", true);
        
        Jet::ComponentPtr window(new Jet::Component("Config"));
        window->value("width", 800);
        window->value("height", 600);
        window->value("title", "Extreme Asteroids");
        window->value("fullscreen", 0.0);
        
        Jet::EnginePtr engine(new Jet::Engine);
        engine->component("shadows", shadow.get());
        engine->component("window", window.get());
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
        engine->resource("Box.obj");
        
        Jet::NodePtr box_node(engine->root()->node("box"));
        box_node->component("box", "Box.obj");
        
        Jet::ComponentPtr light(new Jet::Component("PointLight"));
        light->value("ambient", Jet::Color(0.0f, 0.0f, 0.0f, 1.0f));
        light->value("diffuse", Jet::Color(1.0f, 1.0f, 1.0f, 1.0f));
        light->value("specular", Jet::Color(0.0f, 0.0f, 1.0f, 1.0f));
        
        Jet::NodePtr light_node(engine->root()->node("light"));
        light_node->component("light", light.get());
        light_node->position(Jet::Vector(10.0f, 0.0f, 0.0f));
        

        //engine->module("Jet.Direct3D9");
        //engine->module("Jet.FMOD");
        //engine->module("Jet.Lua");
        //engine->module("Jet.Python");
        //enigne->module("Jet.XML");
    
        while (engine->running()) {
            engine->tick();
        }
        
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}
