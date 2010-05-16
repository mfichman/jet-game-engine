#include <Jet/Engine.hpp>
#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>

#include <Jet/Core/NetworkSystem.hpp>
#include <Jet/Core/Socket.hpp>
#include <Jet/Core/SocketWriter.hpp>
#include <Jet/Core/SocketReader.hpp>

using namespace std;
using namespace boost;

int main(int argc, char** argv) {
	/*
#if defined(WINDOWS) && defined(_DEBUG)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	cout << UINT_MAX << endl;

    try {        
        Jet::EnginePtr engine(Jet::Engine::create()); 
        
        while (engine->running()) {
			engine->update();
		}
       	return 0; 
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
		return 1;
    }*/
	
	try {
		
		using namespace Jet;
		Core::NetworkSystemPtr system(new Core::NetworkSystem(0));
		/*
		char choice;
		cin >> choice;
		if (choice == 's') {
			Core::SocketPtr socket(Core::Socket::server(6789));
			size_t count = 10;
			while (true) {
				if (Core::SocketReaderPtr reader = socket->reader()) {
					cout << reader->string() << endl;
				}
				if (Core::SocketWriterPtr writer = socket->writer()) {
					cout << "Sending" << endl;
					writer->string("Hello");
					count--;
				}
				if (count <= 0) {
					//return 0;
				}
				Sleep(100);
			}
		} else {
			Core::SocketPtr socket(Core::Socket::client("127.0.0.1", 6789));
			while (true) {
				if (Core::SocketReaderPtr reader = socket->reader()) {
					cout << reader->string() << endl;
				}
				if (Core::SocketWriterPtr writer = socket->writer()) {
					writer->string("Goodbye");
				}
			}
		}*/

		
		
		Core::SocketPtr socket(Core::Socket::multicast("228.5.6.7", 6789));
		srand(GetTickCount());
		stringstream ss;
		ss << "Hello, world!" << endl;
		ss << rand();


		while (true) {
			while (Core::SocketReaderPtr reader = socket->reader()) {
				cout << reader->string() << endl;
			}
			if (Core::SocketWriterPtr writer = socket->writer()) {
				Sleep(1000);
				cout << "Sending..." << endl;

				writer->string(ss.str());
			}
		}
	} catch (std::exception& ex) {
		cout << ex.what() << endl;
	}
	
	cout.flush();
	
	
	return 0;
}
