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
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Types/Player.hpp>
#include <Jet/Types/Match.hpp>
#include <Jet/Network.hpp>
#include <set>

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
    inline NetworkMonitor* network_monitor() const {
        return 0;
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
    inline const Match& match(size_t index) const {
        return match_[index];
    }
    
    //! Returns information about a player in the game.
    inline const Player& player(size_t index) const {
        return player_[index];
    }
    
    //! Returns information about the current local match
    inline const Match& current_match() const {
        return current_match_;
    }
    
    //! Returns information about the current player.
    inline const Player& current_player() const {
        return current_player_;
    }
    
    //! Sets the current mode of the network
    void state(NetworkState state);
    
    //! Sets the current game.
    inline void current_match(const Match& match) {
        current_match_ = match;
    }
    
    //! Sets information about the current player.
    inline void current_player(const Player& player) {
        current_player_ = player;
    }
    
private:
    void on_tick();
    void on_update();
    void on_render() {}
    void on_init();
    
    void read_rpcs(BSockSocket* socket);
    
    void enter_discover();
    void enter_host();
    void enter_join();
    
    void do_discover();
    void do_host();
    void do_join();
    
    void rpc_match_info(BSockSocket* socket);
    void rpc_match_destroy(BSockSocket* socket);
    void rpc_player_join(BSockSocket* socket);
    void rpc_player_leave(BSockSocket* socket);
    void rpc_player_list(BSockSocket* socket);
    void rpc_match_destroy_all();
    void rpc_player_list_all();

    void on_match_info(BSockReader* reader);
    void on_match_destroy(BSockReader* reader);
    void on_player_join(BSockReader* reader);
    void on_player_leave(BSockReader* reader);
    void on_player_list(BSockReader* reader);
    
    CoreEngine* engine_;
    float accumulator_;
    NetworkState state_;
    std::map<BSockSocketPtr, size_t> socket_;
    std::vector<Match> match_;
    std::vector<Player> player_;
    Match current_match_;
    Player current_player_;
    
    BSockSocketPtr multicast_;
    BSockSocketPtr client_;
    BSockServerSocketPtr server_;

};

}
