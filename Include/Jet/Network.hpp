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
    //! Creates a new network monitor.
    virtual NetworkMonitor* network_monitor(Node* parent)=0;
    
    //! Returns the current network state
    virtual NetworkState state() const=0;
    
    //! Returns the number of games available.
    virtual size_t match_count() const=0;
    
    //! Returns the number of players in the current game.
    virtual size_t player_count() const=0;
    
    //! Returns information about a game.
    virtual const NetworkMatch& match(size_t index) const=0;
    
    //! Returns information about a player in the game.
    virtual const Player& player(size_t index) const=0;
    
    //! Returns information about the current local match
    virtual const NetworkMatch& current_match() const=0;
    
    //! Returns information about the current player.
    virtual const Player& current_player() const=0;
    
    //! Sets the current mode of the network
    virtual void state(NetworkState state)=0;
    
    //! Sets the current game.
    virtual void current_match(const NetworkMatch& match)=0;
    
    //! Sets information about the current player.
    virtual void current_player(const Player& player)=0;
};
    
}