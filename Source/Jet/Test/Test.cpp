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
	using namespace Jet;
	Core::NetworkSystemPtr system(new Core::NetworkSystem(0));
	
	char choice;
	cin >> choice;
	if (choice == 's') {
		Core::SocketPtr socket(Core::Socket::server(6789));
		while (!socket->connected()) {
			cout << "Waiting" << endl;
			Sleep(1000);
		}
		for (int i = 0; i < 1; i++) {
			Sleep(100);
			Core::SocketWriter writer(socket.get());
			writer.string("hello");
		}
	} else {
		Core::SocketPtr socket(Core::Socket::client("127.0.0.1", 6789));
		while (!socket->connected()) {
			cout << "Waiting" << endl;
			Sleep(1000);
		}
		while (socket->connected()) {
			Sleep(100);
			if (socket->incoming_packet()) {
				Core::SocketReader reader(socket.get());
				cout << reader.string() << endl;
			}
		}
	}
	
	
	/*size_t count = 0;

	Core::SocketPtr socket(Core::Socket::multicast("228.5.6.7", 6789));

	
	while (true) {
		Sleep(100);
		cout << "sending" << endl;
		if (count < 10) {
			Core::SocketWriter writer(socket.get());
			writer.string("Hello, world!!");
		} else {
			Core::SocketWriter writer(socket.get());
			writer.string("Goodbye");
		}
		count++;
		
		if (socket->incoming_packet()) {
			Core::SocketReader reader(socket.get());
			std::string hello = reader.string();
			cout << hello << endl;
			if (hello == "Goodbye") {
				return 0;
			}
		}
	}*/
	return 0;
}
