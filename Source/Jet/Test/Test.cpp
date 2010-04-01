#include <Jet/Engine.hpp>
#include <iostream>
#include <stdexcept>

void main() {
    try {
        Jet::EnginePtr engine(new Jet::Engine);
        engine->module("Jet.Core");
        engine->module("Jet.OpenGL");
        engine->module("Jet.Lua");
        
        
        engine->folder("../Scripts");
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
