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
    
private:
    void on_tick();
    void on_update();
    void on_render() {}
    void on_init();
    
    void update_state();
    void read_rpcs(BSockSocket* socket);
    
    void do_discover();
    void do_host();
    void do_join();
    
    void rpc_game_info(BSockSocket* socket);
    void rpc_game_destroy(BSockSocket* socket);
    void rpc_player_join(BSockSocket* socket);
    void rpc_player_leave(BSockSocket* socket);
    void rpc_player_list(BSockSocket* socket);
    void rpc_player_list_all();

    void on_game_info(BSockReader* reader);
    void on_game_destroy(BSockReader* reader);
    void on_player_join(BSockReader* reader);
    void on_player_leave(BSockReader* reader);
    void on_player_list(BSockReader* reader);
    
    CoreEngine* engine_;
    NetworkState state_;
    std::set<BSockGame> game_;
    std::map<BSockSocketPtr, size_t> socket_;
    std::vector<BSockPlayer> player_;
    size_t player_count_;
    
    BSockSocketPtr multicast_;
    BSockSocketPtr client_;
    BSockServerSocketPtr server_;
    float accumulator_;
};

}