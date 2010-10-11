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
#pragma once

#include <Jet/Network/BSockTypes.hpp>
#include <Jet/Network/BSockSocket.hpp>
#include <Jet/Network/BSockServerSocket.hpp>
#include <Jet/Network/BSockNetworkMonitor.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Types/Player.hpp>
#include <Jet/Types/NetworkMatch.hpp>
#include <Jet/Network.hpp>
#include <queue>
#include <map>
#include <vector>

namespace Jet {

//! Controls network options for the engine.
//! @class Network
//! @brief Sets network options for the engine.
class BSockNetwork : public Network, public EngineListener {
public:
    //! Creates a new network system.
    BSockNetwork(CoreEngine* engine);
    
    //! Destroys the network system.
    ~BSockNetwork();
    
    //! Creates a new network monitor.
    inline BSockNetworkMonitor* network_monitor(Node* node) {
        BSockNetworkMonitorPtr monitor(new BSockNetworkMonitor(engine_, static_cast<CoreNode*>(node)));
		network_monitor_[monitor->hash()] = monitor;
		return monitor.get();
    }


    
    //! Returns the current network state
    inline NetworkState state() const {
        return state_;
    }
    
    //! Returns the number of games available.
    inline size_t match_count() const {
        return match_.size();
    }
    
    //! Returns the number of players in the current game.
    inline size_t player_count() const {
        return player_.size();
    }
    
    //! Returns information about a game.
    inline const NetworkMatch& match(size_t index) const {
        return match_[index];
    }
    
    //! Returns information about a player in the game.
    inline const Player& player(size_t index) const {
		if (index > player_.size()) {
			throw std::runtime_error("Player does not exist");
		}
        return player_[index];
    }
    
    //! Returns information about the current local match
    inline const NetworkMatch& current_match() const {
        return current_match_;
    }
    
    //! Returns information about the current player.
    inline const Player& current_player() const {
        return current_player_;
    }
    
    //! Sets the current mode of the network
    void state(NetworkState state);
    
    //! Sets the current game.
    inline void current_match(const NetworkMatch& match) {
        current_match_ = match;
    }
    
    //! Sets information about the current player.
    inline void current_player(const Player& player) {
        current_player_ = player;
    }
    
	//! Invokes an unreliable RPC on all connected machines.  One should keep the
	//! number of arguments to a minimum to conserve bandwidth.  Only number, Vector, 
	//! Quaternion, and string types are permitted.
	//! @param name the name of the RPC
	//! @param args the arguments to invoke
	void unreliable_rpc(const std::string& name, const std::vector<boost::any>& args);

	//! Invokes a reliable RPC on all connected machines.  Note that 
	//! reliable RPCs should not be used frequently, because they are slow 
	//! and can cause packet loss in the low-latency UDP packet stream as
	//! a side-effect of the interaction between UDP and TCP.
	void reliable_rpc(const std::string& name, const std::vector<boost::any>& args);

private:
	// System initialization functions
    void on_tick();
    void on_update();
    void on_render() {}
    void on_init();
    
	// State initialization functions
    void enter_discover();
    void enter_host();
    void enter_client();
    
	// State update functions
    void do_discover();
    void do_host();
    void do_client();
    
	// RPC send functions
    void rpc_match_info(BSockSocket* socket);
    void rpc_match_destroy(BSockSocket* socket);
    void rpc_player_join(BSockSocket* socket);
    void rpc_player_leave(BSockSocket* socket);
    void rpc_player_list(BSockSocket* socket);
	void rpc_sync_tick(BSockSocket* socket);
	void rpc_state(BSockSocket* socket);
	void rpc_input(BSockSocket* socket);
	void rpc_ping(BSockSocket* socket);
	void rpc_user_rpc(BSockSocket* socket, const std::string& name, const std::vector<boost::any>& args);
    void rpc_match_destroy_all();
    void rpc_player_list_all();

	// RPC response functions    
    void read_rpcs(BSockSocket* socket);
    void on_match_info(BSockReader* reader);
    void on_match_destroy(BSockReader* reader);
    void on_player_join(BSockReader* reader);
    void on_player_leave(BSockReader* reader);
    void on_player_list(BSockReader* reader);
	void on_sync_tick(BSockReader* reader);
	void on_state(BSockReader* reader);
	void on_input(BSockReader* reader);
	void on_ping(BSockReader* reader);
	void on_user_rpc(BSockReader* reader);

	void update_stats();
    
    CoreEngine* engine_;

	// Various recordkeeping structures
    float accumulator_;
    NetworkState state_;
    std::vector<NetworkMatch> match_;
    std::vector<Player> player_;
    NetworkMatch current_match_;
    Player current_player_;

	uint32_t tx_bytes_;
	uint32_t rx_bytes_;
	float stats_elapsed_time_;
    
	// Sockets used by the network engine
	std::vector<BSockSocketPtr> stream_;
    BSockSocketPtr multicast_;
	BSockSocketPtr datagram_;
    BSockServerSocketPtr server_;

	// This map holds all the network monitors for objects
	// displayed on the screen
	std::map<uint32_t, BSockNetworkMonitorPtr> network_monitor_;

	friend class BSockSocket;
};

}
