#include <Jet/Engine.hpp>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    try {        
        Jet::EnginePtr engine(Jet::Engine::create()); 
        
        while (engine->running()) {
			engine->tick();
		}
       	return 0; 
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
		return 1;
    }
}
