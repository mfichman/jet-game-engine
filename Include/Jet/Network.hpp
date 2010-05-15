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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>

namespace  Jet {

//! Controls network options for the engine.
//! @class Network
//! @brief Sets network options for the engine.
class Network : public virtual Object {
public:
    
    //! Returns the current user name.
    virtual const std::string& current_player() const=0;
    
    //! Returns the current game name.
    virtual const std::string& current_game() const=0;

    //! Returns the name of the player.
    virtual const std::string& player(size_t index)=0;
    
    //! Returns the name of a game.
    virtual const std::string& game(size_t index)=0;
    
    //! Gets the number of players in the current game.
    virtual size_t player_count() const=0;
        
    //! Returns the number of games available since the last network refresh.
    virtual size_t game_count() const=0;
    
    //! Returns the state of the network interface.  The state can be one
    //! of the following: DISCOVER (to search for new games), HOST (to host
    //! a game), or PLAY (when the game is actually taking place), or DISABLED
    //! (the network is not running)
    virtual NetworkState state() const=0;
    
    //! Sets the current player name.
    virtual void current_player(const std::string& name)=0;
    
    //! Sets the game name, if the user is in the HOST state, or joins a game
    //! if the player is in the discover state.
    virtual void current_game(const std::string& name)=0;
    
    //! Sets the network state.
    virtual void state(NetworkState state)=0;
    
    //! Sends a signal to the running module on the remote machines and the
    //! local machine so that the signal will occur simultaneously on all
    //! machines.
    virtual void signal(const Signal& signal)=0;
};
    
}