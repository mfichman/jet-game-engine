/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <Jet/Sockets/NetworkSystem.hpp>
#include <Jet/Sockets/SocketReader.hpp>
#include <Jet/Sockets/SocketWriter.hpp>
#include <Jet/Sockets/Game.hpp>
#include <Jet/Sockets/Player.hpp>
#include <Jet/Core/Engine.hpp>

#ifdef WINDOWS
#include <winsock2.h>
#endif

using namespace Jet;
using namespace std;

Sockets::NetworkSystem::NetworkSystem(Core::Engine* engine) :
    engine_(engine),
    accumulator_(0.0f),
	state_(DISABLED) {
        
    engine_->option("network_mode", string("disabled"));
    engine_->option("network_player", string("anonymous"));
    engine_->option("network_game", string("unknown"));
    engine_->option("broadcast_rate", 1.0f);
    engine_->option("discover_ip", string("228.5.6.7"));
    engine_->option("discover_port", (float)6789);
	engine_->option("max_players", (float)2);
    
#ifdef WINDOWS
    WSAData data;
    if (WSAStartup(MAKEWORD(1, 1), &data)) {
        throw std::string("Could not start Winsock");
    }
#endif

    engine_->listener(this);
}

Sockets::NetworkSystem::~NetworkSystem() {
#ifdef WINDOWS
    WSACleanup();
#endif
}

void Sockets::NetworkSystem::on_init() {
    
}

void Sockets::NetworkSystem::on_tick() {
}

void Sockets::NetworkSystem::on_update() {
    update_state();
	//try {
		switch (state_) {
			case DISCOVER: do_discover(); break;
			case HOST: do_host(); break;
			case JOIN: do_join(); break;
		}
	//} catch (std::runtime_error& ex) {
	//	cout << ex.what();
	//}
}

void Sockets::NetworkSystem::do_discover() {
    // Read in a game structure from the user
    while (SocketReaderPtr reader = multicast_->reader()) {
        Game game;
        game.name = reader->string();
        game.server_port = (uint16_t)reader->integer();
        game.server_address = reader->string();
        game.last_time = engine_->frame_time();
        if (game_.insert(game).second && engine_->module()) {
            engine_->module()->on_game_found(game.name);
        }
    }
}

void Sockets::NetworkSystem::do_host() {

    // Send the hello message
    accumulator_ += engine_->frame_delta();
    if (accumulator_ > 1.0f/engine_->option<float>("broadcast_rate")) {
        // Write a game data structure
        if (SocketWriterPtr writer = multicast_->writer()) {
            writer->string(engine_->option<string>("network_game"));
            writer->integer(server_->port());
            writer->string(server_->address());
        }
		// Write information about the players to the clients
		for (list<SocketPtr>::iterator j = socket_.begin(); j != socket_.end(); j++) {
            for (set<Player>::iterator i = player_.begin(); i != player_.end(); i++) {
                if (SocketWriterPtr writer = (*j)->writer()) {
                    writer->string(i->name);
                }
            }
		}

        accumulator_ = 0;
    }

    // Accept a new socket if the game is not full
    while (SocketPtr socket = server_->socket()) {
		if (socket_.size() < engine_->option<float>("max_players")) {
			socket_.push_back(socket);
		}
    }

	// For each socket, read information about players
	for (std::list<SocketPtr>::iterator i = socket_.begin(); i != socket_.end(); i++) {
		do_read_player(i->get());
	}
}

void Sockets::NetworkSystem::do_join() {
	// Send 
	accumulator_ += engine_->frame_delta();
	if (accumulator_ > 1.0f/engine_->option<float>("broadcast_rate")) {
		if (SocketWriterPtr writer = client_->writer()) {
			writer->string(engine_->option<string>("network_player"));
		}
        accumulator_ = 0;
	}

	// For each socket, read information about other players
	do_read_player(client_.get());
}

void Sockets::NetworkSystem::update_state() {
    string mode = engine_->option<string>("network_mode");
    NetworkState state;
    if ("discover" == mode) {
        state = DISCOVER;
    } else if ("host" == mode) {
        state = HOST;
    } else if ("disabled" == mode) {
        state = DISABLED;
    } else if ("join" == mode) {
        state = JOIN;
    } else {
        return;
    }
    
    if (state_ == state) {
        return;
    }

	// Joining game, connect to server
	if (JOIN == state) {
	    for (std::set<Game>::iterator i = game_.begin(); i != game_.end(); i++) {
            if (i->name == engine_->option<string>("network_game")) {
                cout << "Joining " << i->name << " " << i->server_address << ":" << i->server_port << endl;
                client_.reset(Socket::client(i->server_address, i->server_port));
            }
        }
	}
    
    // If leaving state, disable the socket
    if (JOIN == state_) {
        client_.reset();
    }
    
	// If in the discover or host state, make sure we have
	// a socket open and connected to the shared multicast address
	// for game discovery
    if (DISCOVER == state || HOST == state && !multicast_) {
        string ip = engine_->option<string>("discover_ip");
        uint16_t port = (uint16_t)engine_->option<float>("discover_port");
        multicast_.reset(Socket::multicast(ip, port));
    }
    
    // If in host mode, enable the server socket on a random port
    if (HOST == state) {
		server_.reset(ServerSocket::server(0));
        cout << "Listening on " << server_->address() << ":" << server_->port() << endl;
    }
    
    // Add the player to the player list and reset it
    if (HOST == state) {
        Player player;
		player.name = engine_->option<string>("network_player");
        player.socket = 0;
        player_.clear();
		player_.insert(player);
        if (engine_->module()) {
            engine_->module()->on_player_join(player.name);
        }
    }
    
    // If not in host mode, shut down the server socket
    if (HOST == state_) {
        server_.reset();
    }

	// If not in the discover or host state, then close down the socket.
	if (DISCOVER != state && HOST != state) {
		multicast_.reset();
		game_.clear();
	}

	state_ = state;
}

void Sockets::NetworkSystem::do_read_player(Socket* socket) {
	// Read a player message and save it
	while (SocketReaderPtr reader = socket->reader()) {
		Player player;
		player.name = reader->string();
		player.socket = socket;
		player.last_time = engine_->frame_time();
		if (player_.insert(player).second && engine_->module()) {
			engine_->module()->on_player_join(player.name);
		}
	}
}