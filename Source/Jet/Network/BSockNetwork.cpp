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
#include <Jet/Types/NetworkMatch.hpp>
#include <Jet/Types/InputState.hpp>
#include <cfloat>
#include <boost/date_time/c_time.hpp>

#ifdef WINDOWS
#include <winsock2.h>
#endif

using namespace Jet;
using namespace std;

BSockNetwork::BSockNetwork(CoreEngine* engine) :
    engine_(engine),
    accumulator_(0.0f),
	state_(NS_DISABLED),
	tx_bytes_(0),
	rx_bytes_(0),
	stats_elapsed_time_(0) {
        
    engine_->option("broadcast_rate", 1.0f);
    engine_->option("discover_ip", string("228.5.6.7"));
    engine_->option("discover_port", (float)6789);
	engine_->option("max_players", (float)3);
	engine_->option("network_smoothness", 0.5f);
	engine_->option("input_delay", (float)12); // Delay input by 12 ticks before processing
	engine_->option("network_packet_rate", (float)6); // Send 1 packet every 6 ticks
	engine_->option("stat_tx_rate", (float)0);
	engine_->option("stat_rx_rate", (float)0);
    
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
	try {
		uint32_t packet_rate = (uint32_t)engine_->option<float>("network_packet_rate");
		if (engine_->tick_id() % packet_rate == 0) {
			
			// Send and packet every 6th physics tick (i.e., every 100ms)
			if (NS_HOST == state_) {
				rpc_state(datagram_.get());
			}

			// Send input to other hosts
			if (NS_HOST == state_ || NS_CLIENT == state_) {
				rpc_input(datagram_.get());
			}
		}

		// Process network monitors
		for (map<uint32_t, BSockNetworkMonitorPtr>::iterator i = network_monitor_.begin(); i != network_monitor_.end(); i++) {
			CoreNode* node = i->second->parent();
			if (node) {
				// Update the network monitor (this copies the state from the network
				// over to the node)
				i->second->tick();

			} else {
				// The node no longer exists, so delete the network monitor
				network_monitor_.erase(i);
			}
		}

	} catch (std::exception&) {
		engine_->module()->on_network_error();
	}
}

void BSockNetwork::on_update() {
	try {
		switch (state_) {
			case NS_DISCOVER: do_discover(); break;
			case NS_HOST: do_host(); break;
			case NS_CLIENT: do_client(); break;
			default: break;
		}
	} catch (std::exception&) {
		if (engine_->module()) {
			engine_->module()->on_network_error();
		}
	}

	update_stats();
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
		rpc_sync_tick(datagram_.get());
		accumulator_ = 0.0f;
    }
    
    // Check for incoming connections
    if (BSockSocketPtr socket = server_->socket()) {
        // If there is an open slot, then assign the
        // newly accepted socket to it.  Otherwise,
        // the socket will close.
        for (size_t i = 1; i < player_.size(); i++) {
            if (!stream_[i]) {
				// Set up the new socket and player
				stream_[i].reset(socket.get());
                player_[i].timestamp = engine_->frame_time();
				rpc_sync_tick(socket.get());
				break;
			}
        }
    }
    
    // Update the sockets
	for (size_t i = 0; i < player_.size(); i++) {
		if (stream_[i]) {
			
			try {
				// Try to send data and/or receive RPCs on the stream socket
				stream_[i]->poll_write();
				read_rpcs(stream_[i].get());
			} catch (std::exception&) {
				// Erase the socket if an error occurs
				stream_[i] = 0;
				player_[i] = Player();

				// Notify the other players that the player has
				// disconnected
				if (engine_->module()) {
					engine_->module()->on_player_list_update();
				}
				rpc_player_list_all();
			}
		}
    }

	// Check for messages on the UDP socket
	datagram_->poll_write();
	read_rpcs(datagram_.get());
}

void BSockNetwork::do_client() {
    // Check for incoming messages from the server
	// Send buffered input keystrokes here if possible
    stream_[0]->poll_write();
    read_rpcs(stream_[0].get());

	// Check for incoming messages from the UDP socket
	datagram_->poll_write();
	read_rpcs(datagram_.get());
}

void BSockNetwork::state(NetworkState state) {    
    if (state_ == state) {
        return;
	}
    
	// Destroy the old client state
    if (NS_CLIENT == state_) {
		try {
			rpc_player_leave(stream_[0].get());
		} catch (std::exception&) {
		}
		stream_.clear();
		datagram_.reset();
    } else if (NS_HOST == state_) {
        // Notify all that the match is destroyed
		try {
			rpc_match_destroy(multicast_.get());
			rpc_match_destroy_all();
		} catch (std::exception&) {
		}
        server_.reset();
		datagram_.reset();
		stream_.clear();
	} else if (NS_DISCOVER != state_) {
		multicast_.reset();
	}
	
	// Initialize the new state
	if (NS_CLIENT == state) {
        enter_client();
	} else if (NS_HOST == state) {
        enter_host();
    } else if (NS_DISCOVER == state) {
        enter_discover();
    }

	state_ = state;
}

void BSockNetwork::enter_discover() {
    
    //! Initialize the multicast socket		
    string ip = engine_->option<string>("discover_ip");
    uint16_t port = (uint16_t)engine_->option<float>("discover_port");
    multicast_.reset(BSockSocket::multicast(engine_, Address(ip, port)));
    
    // Clear the match list
    match_.clear();
}

void BSockNetwork::enter_host() {
    
    // Initialize the server socket
    server_.reset(BSockServerSocket::server(engine_, 0));

	// Seed the random number generator
	::srand((uint32_t)time(NULL));

	// Initialize the datagram socket to send and receive on a
	// random port with a random multicast group
	uint32_t rand_ip = 0xE4000100 | (::rand() & 0xFF);
	datagram_.reset(BSockSocket::multicast(engine_, Address(rand_ip, 0)));
    
    // Initialize the discover multicast socket
    string ip = engine_->option<string>("discover_ip");
    uint16_t port = (uint16_t)engine_->option<float>("discover_port");
    multicast_.reset(BSockSocket::multicast(engine_, Address(ip, port)));
    
    // Clear the match list
    match_.clear();
    
    // Clear and reset the player list
    player_.clear();
    player_.resize((size_t)engine_->option<float>("max_players"));
    player_[0] = current_player_;

	// Clear out client sockets
	stream_.clear();
	stream_.resize(player_.size());
    
    // Update the player list
    if (engine_->module()) {
        engine_->module()->on_player_list_update();
    }
    accumulator_ = FLT_MAX;
}

void BSockNetwork::enter_client() {
	// Resize the stream sockets
	stream_.clear();
	stream_.resize(1);
    
    // Create a new client socket to connect to the server
	stream_[0].reset(BSockSocket::client(engine_, current_match_.stream_address));
	
	// Initialize the datagram socket to send to the server's multicast address
	datagram_.reset(BSockSocket::multicast(engine_, current_match_.datagram_address));
	
	rpc_player_join(stream_[0].get());
}

void BSockNetwork::read_rpcs(BSockSocket* socket) {
    while (BSockReaderPtr reader = socket->reader()) {
        PacketType rpc = static_cast<PacketType>(reader->integer());

		switch (rpc) {
			case PT_MATCH_INFO: on_match_info(reader.get()); break;
			case PT_MATCH_DESTROY: on_match_destroy(reader.get()); break;
			case PT_PLAYER_JOIN: on_player_join(reader.get()); break;
			case PT_PLAYER_LEAVE: on_player_leave(reader.get()); break;
			case PT_PLAYER_LIST: on_player_list(reader.get()); break;
			case PT_STATE: on_state(reader.get()); break;
			case PT_PING: on_ping(reader.get()); break;
			case PT_SYNC: on_sync_tick(reader.get()); break;
			case PT_INPUT: on_input(reader.get()); break;
		}
    }
}

void BSockNetwork::rpc_match_info(BSockSocket* socket) {
    if (BSockWriterPtr writer = socket->writer()) {
        writer->integer(PT_MATCH_INFO);
        writer->string(current_match_.name);
        writer->integer(server_->address().address);
        writer->integer(server_->address().port); // Port
        writer->integer(datagram_->address().address);
        writer->integer(datagram_->address().port); // Multicast port
        writer->integer(current_match_.uuid);     
    }
}

void BSockNetwork::rpc_match_destroy(BSockSocket* socket) {
    if (BSockWriterPtr writer = socket->writer()) {
        writer->integer(PT_MATCH_DESTROY);
        writer->integer(current_match_.uuid);
    }
}

void BSockNetwork::rpc_player_join(BSockSocket* socket) {
    if (BSockWriterPtr writer = socket->writer()) {
        writer->integer(PT_PLAYER_JOIN);
        writer->string(current_player_.name);
        writer->integer(current_player_.uuid);
    }
}

void BSockNetwork::rpc_player_leave(BSockSocket* socket) {
    if (BSockWriterPtr writer = socket->writer()) {
        writer->integer(PT_PLAYER_LEAVE);
        writer->integer(current_player_.uuid);
    }
}

void BSockNetwork::rpc_player_list(BSockSocket* socket) {
    if (BSockWriterPtr writer = socket->writer()) {
        writer->integer(PT_PLAYER_LIST);
        writer->integer(player_.size());
        for (size_t i = 0; i < player_.size(); i++) {
            writer->string(player_[i].name);
            writer->integer(player_[i].uuid);
        }
    }
}

void BSockNetwork::rpc_state(BSockSocket* socket) {
	if (BSockWriterPtr writer = socket->writer()) {
		
		writer->integer(PT_STATE); // Write the packet type
		writer->integer(engine_->tick_id()); // Tick this state was sent on
		writer->integer(current_player_.uuid);
		
		// Write state for each actor in the 
		for (map<uint32_t, BSockNetworkMonitorPtr>::iterator i = network_monitor_.begin(); i != network_monitor_.end();) {
			
			// Increment the iterator in case we need to delete the node.
			// This prevents the iterator from being invalidated.
			map<uint32_t, BSockNetworkMonitorPtr>::iterator j = i++;
			CoreNode* node = j->second->parent();
			uint32_t uuid = j->second->player_uuid();
			
			// Write the most current state information for the node. This includes 
			// position, velocity,  and rotation.  If a UDP socket is used, and the
			// previous buffer has not already been sent, then this will overwrite 
			// the waiting buffer.  There's no point in sending data that is out
			// of date.
			if (!node) {
				// The node has been destroyed, so erase the network monitor.
				// This releases the reference on the monitor, thus freeing it.
				network_monitor_.erase(j);
			} else if (node->visible() && (!uuid || uuid == current_player_.uuid)) {
				// If the node is visible and owned by the local player, then broadcast
				// information about the node to all other players
				
				writer->integer(j->first); // Node hash (for ID)
				writer->integer(node->actor()->state_hash()) ;// Write state hash 

				const Vector& position = node->position();
				writer->real(position.x);
				writer->real(position.y);
				writer->real(position.z);

				const Quaternion& rotation = node->rotation();
				writer->real(rotation.w);
				writer->real(rotation.x);
				writer->real(rotation.y);
				writer->real(rotation.z);

				const Vector& linear_velocity = node->linear_velocity();
				writer->real(linear_velocity.x);
				writer->real(linear_velocity.y);
				writer->real(linear_velocity.z);

				const Vector& angular_velocity = node->angular_velocity();
				writer->real(angular_velocity.x);
				writer->real(angular_velocity.y);
				writer->real(angular_velocity.z);
			}
		}

		// Write the end-of-packet marker: a zero word
		writer->integer(0);
	}
}

void BSockNetwork::rpc_input(BSockSocket* socket) {
	if (BSockWriterPtr writer = socket->writer()) {
		const InputState& state = engine_->input()->input_state();

		writer->integer(PT_INPUT);
		writer->integer(state.player_uuid); // Write the player UUID
		writer->integer(state.tick); // Active tick
		writer->integer(state.mouse_button);
		writer->real(state.mouse.x);
		writer->real(state.mouse.y);

		uint32_t words = (uint32_t)ceil((float)state.key.size()/(float)32);
		writer->integer(words);

		// Compress the key state by sending a bitset instead of
		// the whole array of byte-flags
		uint32_t word = 0;
		size_t i;
		for (i = 0; i < state.key.size(); i++) {
			uint32_t bit = i % 32;		
			if (state.key[i]) {
				word |= 1 << bit;
			}
			if ((i % 32) ==  31) {
				writer->integer(word);
				word = 0;
			}
		}
		if ((i % 32) != 0) {
			writer->integer(word);
		}
	}
}

void BSockNetwork::rpc_ping(BSockSocket* socket) {
	if (BSockWriterPtr writer = socket->writer()) {
		cout << "Sending ping" << endl;
		writer->integer(PT_PING);
	}
}

void BSockNetwork::rpc_sync_tick(BSockSocket* socket) {
	if (BSockWriterPtr writer = socket->writer()) {
		writer->integer(PT_SYNC);
		writer->integer(engine_->tick_id());
	}
}



void BSockNetwork::rpc_player_list_all() {      
    // Update the player list for all the players
	for (size_t i = 0; i < stream_.size(); i++) {
		if (stream_[i]) {
			rpc_player_list(stream_[i].get());
		}
    }
}

void BSockNetwork::rpc_match_destroy_all() {
	// Destroy the match
	for (size_t i = 0; i < player_.size(); i++) {
		if (stream_[i]) {
			rpc_match_destroy(stream_[i].get());
		}
	}
}

void BSockNetwork::on_match_info(BSockReader* reader) {
    NetworkMatch match;
    match.name = reader->string();
    match.stream_address.address = reader->integer();
    match.stream_address.port = (uint16_t)reader->integer();
    match.datagram_address.address = reader->integer();
    match.datagram_address.port = (uint16_t)reader->integer();
    match.uuid = reader->integer();
    match.timestamp = engine_->frame_time();

    // Search for and update/add the match to the list
    vector<NetworkMatch>::iterator i = find(match_.begin(), match_.end(), match);
    if (i != match_.end()) {
        *i = match;
    } else {
        match_.push_back(match);
    }
    
    if (engine_->module()) {
        engine_->module()->on_match_list_update();
    }
}

void BSockNetwork::on_match_destroy(BSockReader* reader) {
    NetworkMatch match;
    match.uuid = reader->integer();
    
    // Search for and erase the match
    vector<NetworkMatch>::iterator i = find(match_.begin(), match_.end(), match);
    if (i != match_.end()) {
        match_.erase(i);
    }
    
    if (engine_->module()) {
        engine_->module()->on_match_list_update();
		if (NS_CLIENT == state_ && match == current_match_) {
			engine_->module()->on_network_error();
		}
    }
}

void BSockNetwork::on_player_join(BSockReader* reader) {
    vector<BSockSocketPtr>::iterator j = find(stream_.begin(), stream_.end(), reader->socket());
	if (j != stream_.end()) {
		size_t i = j - stream_.begin();
		player_[i].name = reader->string();
		player_[i].uuid = reader->integer();
		player_[i].timestamp = engine_->frame_time();
		if (engine_->module()) {
			engine_->module()->on_player_list_update();
		}
		rpc_player_list_all();
	}
}

void BSockNetwork::on_player_leave(BSockReader* reader) {
    // Read which player quic, and notify the GUI
    vector<BSockSocketPtr>::iterator j = find(stream_.begin(), stream_.end(), reader->socket());
	if (j != stream_.end()) {
		size_t i = j - stream_.begin();
		stream_[i] = 0;
		player_[i] = Player();
		if (engine_->module()) {
			engine_->module()->on_player_list_update();
		}
		rpc_player_list_all();
	}
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

void BSockNetwork::on_state(BSockReader* reader) {
	uint32_t tick = reader->integer(); // Read the tick ID
	uint32_t uuid = reader->integer();
	uint32_t hash = 0;

	if (uuid == current_player_.uuid) {
		return;
	}

	while (hash = reader->integer()) {
		// Read all the state data in for this node
		uint32_t state_hash = reader->integer();
		Vector position;
		position.x = reader->real();
		position.y = reader->real();
		position.z = reader->real();

		Quaternion rotation;
		rotation.w = reader->real();
		rotation.x = reader->real();
		rotation.y = reader->real();
		rotation.z = reader->real();

		Vector linear_velocity;
		linear_velocity.x = reader->real();
		linear_velocity.y = reader->real();
		linear_velocity.z = reader->real();

		Vector angular_velocity;
		angular_velocity.x = reader->real();
		angular_velocity.y = reader->real();
		angular_velocity.z = reader->real();

		map<uint32_t, BSockNetworkMonitorPtr>::iterator i = network_monitor_.find(hash);
		if (i != network_monitor_.end()) {
			BSockNetworkMonitorPtr network_monitor = i->second;
			network_monitor->state_hash(tick, state_hash);
			network_monitor->position(tick, position, linear_velocity);
			network_monitor->rotation(tick, rotation, angular_velocity);
		}
	}
}

void BSockNetwork::on_input(BSockReader* reader) {

	InputState state;
	state.player_uuid = reader->integer();
	if (state.player_uuid == current_player_.uuid) {
		return;
	}

	state.tick = reader->integer();
	state.mouse_button = reader->integer();
	state.mouse.x = reader->real();
	state.mouse.y = reader->real();

	state.key.resize(reader->integer() * 32);

	uint32_t word;
	for (size_t i = 0; i < state.key.size(); i++) {
		if (i % 32 == 0) {
			word = reader->integer();
		}
		uint32_t bit = i % 32;
		if (word & (1 << bit)) {
			state.key[i] = 1;
		}
	}

	engine_->input()->input_state(state);
}

void BSockNetwork::on_ping(BSockReader* reader) {
	std::cout << "Ping" << std::endl;
}


void BSockNetwork::on_sync_tick(BSockReader* reader) {
	uint32_t tick = reader->integer();
	engine_->tick_id(tick);

}

void BSockNetwork::update_stats() {
	stats_elapsed_time_ += engine_->frame_delta();
	if (stats_elapsed_time_ > 0.5f) {
		engine_->option("stat_rx_rate", (float)rx_bytes_/stats_elapsed_time_*8.0f/1000.0f);
		engine_->option("stat_tx_rate", (float)tx_bytes_/stats_elapsed_time_*8.0f/1000.0f);
		rx_bytes_ = 0;
		tx_bytes_ = 0;
		stats_elapsed_time_ = 0.0f;
	}
}

/*
uint32_t tick_delay = (uint32_t)engine_->option<float>("network_tick_delay");
	uint32_t tick = reader->integer(); // Read the tick ID


	if (tick + tick_delay <= engine_->tick_id()) {
		// Packet is still usable, because it is early or just on time
		// Add it to the list of outstanding packets
		input_state_.push(BSockState(reader, tick + tick_delay));
	}
*/

/*
	uint32_t hash = 0;

	// Calculate the time delta so that we can advance the position and
	// rotation using the state from the server (which is necessarily old)
	float delta = (engine_->tick_id() - tick) * engine_->timestep();// + tick_accumulator_;

	while (hash = reader->integer()) {
		//writer-> Read state hash for the actor, if it exists

		// Read all the state data in for this node
		Vector position(reader->real(), reader->real(), reader->real());
		Quaternion rotation(reader->real(), reader->real(), reader->real(), reader->real());
		Vector linear_velocity(reader->real(), reader->real(), reader->real());
		Vector angular_velocity(reader->real(), reader->real(), reader->real());
		
		// Look up the node by hash
		map<uint32_t, BSockNetworkMonitorPtr>::iterator i = network_monitor_.find(hash);
		if (i != network_monitor_.end()) {
			CoreNode* node = i->second->parent();
			if (node) {
				// Advance the position and rotation by the given time
				position += linear_velocity * delta;
				rotation += (rotation.unit() * Quaternion(angular_velocity * delta, 0) * 0.5f).unit();

				// Do a smooth interpolation between the network position
				// and the current position, so that objects don't appear
				// to jump when a packet arrives
				position = position.lerp(node->position(), alpha);
				rotation = rotation.slerp(node->rotation(), alpha);
				node->position(position);
				node->rotation(rotation);

				// Snap the linear and angular velocity
				node->rigid_body()->linear_velocity(linear_velocity);
				node->rigid_body()->angular_velocity(angular_velocity);

			} else {
				// The node no longer exists, so delete the network monitor
				network_monitor_.erase(i);
			}
		}
	}
*/