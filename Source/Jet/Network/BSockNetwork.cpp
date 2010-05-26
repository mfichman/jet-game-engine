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

#include <Jet/Network/BSockNetwork.hpp>
#include <Jet/Network/BSockReader.hpp>
#include <Jet/Network/BSockWriter.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Types/Player.hpp>
#include <Jet/Types/Match.hpp>
#include <cfloat>

#ifdef WINDOWS
#include <winsock2.h>
#endif

using namespace Jet;
using namespace std;

BSockNetwork::BSockNetwork(CoreEngine* engine) :
    engine_(engine),
    accumulator_(0.0f),
	state_(NS_DISABLED) {
        
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

BSockNetwork::~BSockNetwork() {
#ifdef WINDOWS
    WSACleanup();
#endif
}

void BSockNetwork::on_init() {
    
}

void BSockNetwork::on_tick() {
}

void BSockNetwork::on_update() {
	try {
		switch (state_) {
			case NS_DISCOVER: do_discover(); break;
			case NS_HOST: do_host(); break;
			case NS_JOIN: do_join(); break;
			default: break;
		}
	} catch (std::exception&) {
		if (engine_->module()) {
			engine_->module()->on_network_error();
		}
	}
}

void BSockNetwork::do_discover() {
    // Read in a game structure from the user
    read_rpcs(multicast_.get());
}

void BSockNetwork::do_host() {
    // Send the "hello" message on the multicast socket channel
    accumulator_ += engine_->frame_delta();
    if (accumulator_ > 1.0f/engine_->option<float>("broadcast_rate")) {
        rpc_match_info(multicast_.get());
		accumulator_ = 0.0f;
    }
    
    // Check for incoming connections
    if (BSockSocketPtr socket = server_->socket()) {
        // If there is an open slot, then assign the
        // newly accepted socket to it.  Otherwise,
        // the socket will close.
        for (size_t i = 1; i < player_.size(); i++) {
            if (!player_[i].timestamp) {
                rpc_player_list(socket.get());
                socket_.insert(make_pair(socket, i));
                player_[i].timestamp = engine_->frame_time();

			} else {
				cout << "Match is full" << endl;
			}
        }
    }
    
    // Update the sockets by reading for data
    for (map<BSockSocketPtr, size_t>::iterator i = socket_.begin(); i != socket_.end(); i++) {
		try {
			i->first->poll_write();
			read_rpcs(i->first.get());
		} catch (std::exception&) {
			player_[i->second].timestamp = 0.0f;
			player_[i->second].name = "";
		}
    }
    
    // Check for timed-out players and dead sockets
	for (map<BSockSocketPtr, size_t>::iterator i = socket_.begin(); i != socket_.end();) {
		map<BSockSocketPtr, size_t>::iterator j = i;
		i++;
		if (!player_[j->second].timestamp) {
			socket_.erase(j);
		}
	}
}

void BSockNetwork::do_join() {
    // Check for incoming messages from the server
    client_->poll_write();
    read_rpcs(client_.get());
}

void BSockNetwork::state(NetworkState state) {    
    if (state_ == state) {
        return;
    }

	// Joining match, connect to server
	if (NS_JOIN == state) {
        enter_join();
	}
    
    // If in host mode, enable the server socket on a random port
    if (NS_HOST == state) {
        enter_host();
    }
    
    if (NS_DISCOVER == state) {
        enter_discover();
    }
    
        // If leaving state, disable the socket
    if (NS_JOIN == state_) {
		try {
			rpc_player_leave(client_.get());
		} catch (std::exception&) {
		}
		client_.reset();
    }
    
    // If not in host mode, shut down the server socket
    if (NS_HOST == state_) {
        // Notify all that the match is destroyed..?
		try {
			rpc_match_destroy(multicast_.get());
			rpc_match_destroy_all();
		} catch (std::exception&) {
		}
        server_.reset();
    }
    
    // Leaving the discover state
    if (NS_DISCOVER != state && NS_HOST != state) {
		multicast_.reset();
	}

	state_ = state;
}

void BSockNetwork::enter_discover() {
    
    //! Initialize the multicast socket		
    string ip = engine_->option<string>("discover_ip");
    uint16_t port = (uint16_t)engine_->option<float>("discover_port");
    multicast_.reset(BSockSocket::multicast(ip, port));
    
    // Clear the match list
    match_.clear();
}

void BSockNetwork::enter_host() {
    
    // Initialize the server socket
    server_.reset(BSockServerSocket::server(0));
    
    // Initialize the discover multicast socket
    string ip = engine_->option<string>("discover_ip");
    uint16_t port = (uint16_t)engine_->option<float>("discover_port");
    multicast_.reset(BSockSocket::multicast(ip, port));
    
    // Clear the match list
    match_.clear();
    
    // Clear and reset the player list
    player_.clear();
    player_.resize((size_t)engine_->option<float>("max_players"));
    player_[0] = current_player_;
    
    // Update the player list
    if (engine_->module()) {
        engine_->module()->on_player_list_update();
    }
    accumulator_ = FLT_MAX;
}

void BSockNetwork::enter_join() {
    
    // Create a new client socket to connect to the server
	client_.reset(BSockSocket::client(current_match_.server_address, current_match_.server_port));
    rpc_player_join(client_.get());
}

void BSockNetwork::read_rpcs(BSockSocket* socket) {
    while (BSockSocketReaderPtr reader = socket->reader()) {
        string rpc = reader->string();
        if ("match_info" == rpc) {
            on_match_info(reader.get());
        } else if ("match_destroy" == rpc) {
            on_match_destroy(reader.get());
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

void BSockNetwork::rpc_match_info(BSockSocket* socket) {
    if (BSockSocketWriterPtr writer = socket->writer()) {
        writer->string("match_info");
        writer->string(current_match_.name);
        writer->string(server_->address());
        writer->string("");
        writer->integer(server_->port()); // Port
        writer->integer(0); // Multicast port
        writer->integer(current_match_.uuid);        
    }
}

void BSockNetwork::rpc_match_destroy(BSockSocket* socket) {
    if (BSockSocketWriterPtr writer = socket->writer()) {
        writer->string("match_destroy");
        writer->integer(current_match_.uuid);
    }
}

void BSockNetwork::rpc_player_join(BSockSocket* socket) {
    if (BSockSocketWriterPtr writer = socket->writer()) {
        writer->string("player_join");
        writer->string(current_player_.name);
        writer->integer(current_player_.uuid);
    }
}

void BSockNetwork::rpc_player_leave(BSockSocket* socket) {
    if (BSockSocketWriterPtr writer = socket->writer()) {
        writer->string("player_leave");
        writer->integer(current_player_.uuid);
    }
}

void BSockNetwork::rpc_player_list(BSockSocket* socket) {
    if (BSockSocketWriterPtr writer = socket->writer()) {
        writer->string("player_list");
        writer->integer(player_.size());
        for (size_t i = 0; i < player_.size(); i++) {
            writer->string(player_[i].name);
            writer->integer(player_[i].uuid);
        }
    }
}

void BSockNetwork::rpc_player_list_all() {      
    // Update the player list for all the players
    for (map<BSockSocketPtr, size_t>::iterator i = socket_.begin(); i != socket_.end(); i++) {
        rpc_player_list(i->first.get());
    }
}

void BSockNetwork::on_match_info(BSockReader* reader) {
    Match match;
    match.name = reader->string();
    match.server_address = reader->string();
    match.multicast_address = reader->string();
    match.server_port = (uint16_t)reader->integer();
    match.multicast_port = (uint16_t)reader->integer();
    match.uuid = reader->integer();
    match.timestamp = engine_->frame_time();
    
    // Search for and update/add the match to the list
    vector<Match>::iterator i = find(match_.begin(), match_.end(), match);
    if (i != match_.end()) {
        *i = match;
    } else {
        match_.push_back(match);
    }
    
    if (engine_->module()) {
        engine_->module()->on_match_list_update();
    }
}

void BSockNetwork::rpc_match_destroy_all() {
	// Destroy the match
	for (map<BSockSocketPtr, size_t>::iterator i = socket_.begin(); i != socket_.end(); i++) {
		rpc_match_destroy(i->first.get());
	}
}

void BSockNetwork::on_match_destroy(BSockReader* reader) {
    Match match;
    match.uuid = reader->integer();
    
    // Search for and erase the match
    vector<Match>::iterator i = find(match_.begin(), match_.end(), match);
    if (i != match_.end()) {
        match_.erase(i);
    }
    
    if (engine_->module()) {
        engine_->module()->on_match_list_update();
		if (NS_JOIN == state_ && match == current_match_) {
			engine_->module()->on_network_error();
		}
    }
}

void BSockNetwork::on_player_join(BSockReader* reader) {
    map<BSockSocketPtr, size_t>::iterator j = socket_.find(reader->socket());
    size_t i = j->second;
    player_[i].name = reader->string();
    player_[i].uuid = reader->integer();
    player_[i].timestamp = engine_->frame_time();
    if (engine_->module()) {
        engine_->module()->on_player_list_update();
    }
    rpc_player_list_all();
}

void BSockNetwork::on_player_leave(BSockReader* reader) {
    // Read which player quic, and notify the GUI
    map<BSockSocketPtr, size_t>::iterator j = socket_.find(reader->socket());
    size_t i = j->second;
    player_[i] = Player(); // Reset the player
    if (engine_->module()) {
        engine_->module()->on_player_list_update();
    }
    rpc_player_list_all();
}

void BSockNetwork::on_player_list(BSockReader* reader) {
    player_.resize(reader->integer());
    for (size_t i = 0; i < player_.size(); i++) {
        player_[i].name = reader->string();
        player_[i].uuid = reader->integer();
        player_[i].timestamp = engine_->frame_time();
        if (engine_->module()) {
            engine_->module()->on_player_list_update();
        }
    }
}
