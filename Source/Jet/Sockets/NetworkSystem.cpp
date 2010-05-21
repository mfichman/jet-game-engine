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
    read_rpcs(multicast_.get());
}

void Sockets::NetworkSystem::do_host() {
    // Send the "hello" message on the multicast socket channel
    accumulator_ += engine_->frame_delta();
    if (accumulator_ > 1.0f/engine_->option<float>("broadcast_rate")) {
        rpc_game_info(multicast_.get());
		accumulator_ = 0.0f;
    }
    
    // Check for incoming connections
    if (SocketPtr socket = server_->socket()) {
        // If there is an open slot, then assign the
        // newly accepted socket to it.  Otherwise,
        // the socket will close.
        for (size_t i = 1; i < player_.size(); i++) {
            if (!player_[i].time) {
                rpc_player_list(socket.get());
                socket_.insert(make_pair(socket, i));
                player_[i].time = engine_->frame_time();

			} else {
				cout << "Game is full" << endl;
			}
        }
    }
    
    // Update the sockets by reading for data
    for (map<SocketPtr, size_t>::iterator i = socket_.begin(); i != socket_.end(); i++) {
		try {
			i->first->poll_write();
			read_rpcs(i->first.get());
		} catch (std::exception&) {
			player_[i->second].time = 0.0f;
			player_[i->second].name = "";
		}
    }
    
    // Check for timed-out players and dead sockets
	for (map<SocketPtr, size_t>::iterator i = socket_.begin(); i != socket_.end();) {
		if (!player_[i->second].time) {
			i = socket_.erase(i);
		} else {
			i++;
		}
	}
}

void Sockets::NetworkSystem::do_join() {
    // Check for incoming messages from the server
    client_->poll_write();
    read_rpcs(client_.get());
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
                rpc_player_join(client_.get());
            }
        }
	}
    
    // If leaving state, disable the socket
    if (JOIN == state_) {
        rpc_player_leave(client_.get());
        client_.reset();
    }
    
    // If in host mode, enable the server socket on a random port
    if (HOST == state) {
		server_.reset(ServerSocket::server(0));
        string ip = engine_->option<string>("discover_ip");
        uint16_t port = (uint16_t)engine_->option<float>("discover_port");
        multicast_.reset(Socket::multicast(ip, port));
		player_.clear();
        player_.resize((size_t)engine_->option<float>("max_players"));
        player_[0].name = engine_->option<string>("network_player");
        if (engine_->module()) {
            engine_->module()->on_player_update(0, player_[0].name);
        }
        accumulator_ = FLT_MAX;
        cout << "Listening on " << server_->address() << ":" << server_->port() << endl;
    }
    
    // If not in host mode, shut down the server socket
    if (HOST == state_) {
        // Notify all that the game is destroyed..?
        rpc_game_destroy(multicast_.get());
        multicast_.reset();
        server_.reset();
    }
    
    // If in the discover or host state, make sure we have
	// a socket open and connected to the shared multicast address
	// for game discovery
    if (DISCOVER == state || HOST == state && !multicast_) {
		game_.clear();
        string ip = engine_->option<string>("discover_ip");
        uint16_t port = (uint16_t)engine_->option<float>("discover_port");
        multicast_.reset(Socket::multicast(ip, port));
    }

	// If not in the discover or host state, then close down the socket.
	if (DISCOVER != state && HOST != state) {
		multicast_.reset();
	}

	state_ = state;
}

void Sockets::NetworkSystem::read_rpcs(Socket* socket) {
    while (SocketReaderPtr reader = socket->reader()) {
        string rpc = reader->string();
        if ("game_info" == rpc) {
            on_game_info(reader.get());
        } else if ("game_destroy" == rpc) {
            on_game_destroy(reader.get());
        } else if ("player_join" == rpc) {
            on_player_join(reader.get());
        } else if ("player_leave" == rpc) {
            on_player_leave(reader.get());
        } else if ("player_list" == rpc) {
            on_player_list(reader.get());
        } else {
            cout << "Unknown RPC: " << rpc << endl;
        }
    }
}

void Sockets::NetworkSystem::rpc_game_info(Socket* socket) {
    if (SocketWriterPtr writer = socket->writer()) {
        writer->string("game_info");
        writer->string(engine_->option<string>("network_game"));
        writer->string(server_->address());
        writer->integer(server_->port());
    }
}

void Sockets::NetworkSystem::rpc_game_destroy(Socket* socket) {
    if (SocketWriterPtr writer = socket->writer()) {
        writer->string("game_destroy");
        writer->string(engine_->option<string>("network_game"));
        writer->string(server_->address());
        writer->integer(server_->port());
    }
}

void Sockets::NetworkSystem::rpc_player_join(Socket* socket) {
    if (SocketWriterPtr writer = socket->writer()) {
        writer->string("player_join");
        writer->string(engine_->option<string>("network_player"));
    }
}

void Sockets::NetworkSystem::rpc_player_leave(Socket* socket) {
    if (SocketWriterPtr writer = socket->writer()) {
        writer->string("player_leave");
    }
}

void Sockets::NetworkSystem::rpc_player_list(Socket* socket) {
    if (SocketWriterPtr writer = socket->writer()) {
        writer->string("player_list");
        writer->integer(player_.size());
        for (size_t i = 0; i < player_.size(); i++) {
            writer->string(player_[i].name);
        }
    }
}

void Sockets::NetworkSystem::rpc_player_list_all() {      
    // Update the player list for all the players
    for (map<SocketPtr, size_t>::iterator i = socket_.begin(); i != socket_.end(); i++) {
        rpc_player_list(i->first.get());
    }
}

void Sockets::NetworkSystem::on_game_info(SocketReader* reader) {
    Game game;
    game.name = reader->string();
    game.server_address = reader->string();
    game.server_port = (uint16_t)reader->integer();
    game.time = engine_->frame_time();
    if (game_.insert(game).second && engine_->module()) {
        engine_->module()->on_game_found(game.name);
    }
}

void Sockets::NetworkSystem::on_game_destroy(SocketReader* reader) {
    Game game;
    game.name = reader->string();
    game.server_address = reader->string();
    game.server_port = (uint16_t)reader->integer();
    game_.erase(game);
    if (engine_->module()) {
        engine_->module()->on_game_lost(game.name);
    }
}

void Sockets::NetworkSystem::on_player_join(SocketReader* reader) {
    map<SocketPtr, size_t>::iterator j = socket_.find(reader->socket());
    size_t i = j->second;
    player_[i].name = reader->string();
    player_[i].time = engine_->frame_time();
    if (engine_->module()) {
        engine_->module()->on_player_update(i, player_[i].name);
    }
    rpc_player_list_all();
}

void Sockets::NetworkSystem::on_player_leave(SocketReader* reader) {
    // Read which player quic, and notify the GUI
    map<SocketPtr, size_t>::iterator j = socket_.find(reader->socket());
    size_t i = j->second;
    player_[i].name = "";
    player_[i].time = 0.0f;
    if (engine_->module()) {
        engine_->module()->on_player_update(i, player_[i].name);
    }
    rpc_player_list_all();
}

void Sockets::NetworkSystem::on_player_list(SocketReader* reader) {
    player_.resize(reader->integer());
    for (size_t i = 0; i < player_.size(); i++) {
        player_[i].name = reader->string();
        player_[i].time = engine_->frame_time();
        if (engine_->module()) {
            engine_->module()->on_player_update(i, player_[i].name);
        }
    }
}