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

#include <Jet/Sockets/Types.hpp>
#include <Jet/Sockets/Socket.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Network.hpp>
#include <boost/thread.hpp>

namespace Jet { namespace Sockets {

//! Controls network options for the engine.
//! @class Network
//! @brief Sets network options for the engine.
class NetworkSystem : public Network, public EngineListener {
public:
    //! Creates a new network system.
    NetworkSystem(Core::Engine* engine);
    
    //! Destroys the network system.
    ~NetworkSystem();
    
    //! Returns the current user name.
    inline const std::string& current_player() const {
        return current_player_;
    }
    
    //! Returns the current game name.
    inline const std::string& current_game() const {
        return current_game_;
    }

    //! Returns the name of the player.
    inline const std::string& player(size_t index) {
        return player_[index];
    }
    
    //! Returns the name of a game.
    inline const std::string& game(size_t index) {
        return game_[index];
    }
    
    //! Gets the number of players in the current game.
    inline size_t player_count() const {
        return player_.size();
        
    }
    //! Returns the number of games available since the last network refresh.
    inline size_t game_count() const {
        return game_.size();
    }
    
    //! Returns the state of the network interface.  The state can be one
    //! of the following: DISCOVER (to search for new games), HOST (to host
    //! a game), or PLAY (when the game is actually taking place), or DISABLED
    //! (the network is not running)
    inline NetworkState state() const {
        return state_;
    }
    
    //! Sets the current player name.
    inline void current_player(const std::string& name) {
        current_player_ = name;
    }
    
    //! Sets the game name, if the user is in the HOST state, or joins a game
    //! if the player is in the discover state.
    inline void current_game(const std::string& name) {
        current_game_ = name;
    }
    
    //! Sets the network state.
    void state(NetworkState state);
    
    //! Sends a signal to the running module on the remote machines and the
    //! local machine so that the signal will occur simultaneously on all
    //! machines.
    void signal(const Signal& signal);
    
private:
    void on_tick();
    void on_update();
    void on_render() {}
    void on_init();
    
    Core::Engine* engine_;
    std::string current_player_;
    std::string current_game_;
    std::vector<std::string> player_;
    std::vector<std::string> game_;
    NetworkState state_;
    
    SocketPtr multicast_;
};
    
}}