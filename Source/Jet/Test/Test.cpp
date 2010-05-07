#include <Jet/Engine.hpp>
#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>

#include <Jet/Overlay.hpp>

using namespace std;

int main(int argc, char** argv) {
	
#if defined(WINDOWS) && defined(_DEBUG)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
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
