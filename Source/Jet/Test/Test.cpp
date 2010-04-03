#include <Jet/Engine.hpp>
#include <Jet/Component.hpp>
#include <iostream>
#include <stdexcept>

void main() {
    try {
        Jet::ComponentPtr shadow(new Jet::Component("Config"));
        shadow->value("texture_size", 1024);
        shadow->value("enabled", true);
        
        Jet::ComponentPtr window(new Jet::Component("Config"));
        window->value("width", 1680);
        window->value("height",1050);
        window->value("title", "Extreme Asteroids");
        window->value("fullscreen", true);
        
        Jet::EnginePtr engine(new Jet::Engine);
        engine->component("shadows", shadow.get());
        engine->component("window", window.get());
        engine->folder("../Textures");
        engine->folder("../Meshes");
        engine->folder("../Shaders");
        engine->folder("../Scripts");
        engine->folder("../Sounds");
        engine->folder("../Music");
        engine->module("Jet.Core");
        engine->module("Jet.OpenGL");
        engine->module("Jet.Lua");

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
