#include <Jet/Engine.hpp>
#include <iostream>
#include <stdexcept>

void main() {
    try {        
        Jet::EnginePtr engine(Jet::Engine::create()); 
        
        while (engine->running()) {
			engine->tick();
		}
        
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}