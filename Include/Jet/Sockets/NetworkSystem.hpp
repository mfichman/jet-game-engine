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
#include <Jet/Sockets/ServerSocket.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Network.hpp>
#include <set>

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
    
private:
    void on_tick();
    void on_update();
    void on_render() {}
    void on_init();
    
    void update_state();
    
    void do_discover();
    void do_host();
    void do_join();
    
    void do_read_player(Socket* socket);
    
    Core::Engine* engine_;
    NetworkState state_;
    std::set<Game> game_;
    std::set<Player> player_;
    std::list<SocketPtr> socket_;
    
    SocketPtr multicast_;
    SocketPtr client_;
    ServerSocketPtr server_;
    float accumulator_;
};
    
}}