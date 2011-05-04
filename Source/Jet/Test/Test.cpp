#include <Jet/Engine.hpp>
#include <Jet/Script.hpp>
#include <Jet/Types/Vector.hpp>
#include <Jet/Scene/Node.hpp>
#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>


using namespace std;
using namespace boost;

int main(int argc, char** argv) {
	
#if defined(WINDOWS) && defined(_DEBUG)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    try {
        Jet::EnginePtr engine(Jet::Engine::create()); 
		engine->update();

        while (engine->running()) {
			engine->update();
		}
       	return 0; 
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        system("pause");
		return 1;
    }
	
	
	return 0;
}
