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
#include <Jet/Core/Engine.hpp>

#ifdef WINDOWS
#include <winsock2.h>
#endif

using namespace Jet;
using namespace std;

Sockets::NetworkSystem::NetworkSystem(Core::Engine* engine) :
    engine_(engine),
    current_player_("Anonymous"),
    current_game_("Unknown") {
        
#ifdef WINDOWS
    WSAData data;
    if (WSAStartup(MAKEWORD(1, 1), &data)) {
        throw std::string("Could not start Winsock");
    }
#endif
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
    
}

void Sockets::NetworkSystem::state(NetworkState state) {
    if (state_ == state) {
        return;
    }
    
    if (DISCOVER == state || HOST == state) {
        string ip = engine_->option<string>("discover_ip");
        uint16_t port = engine_->option<uint16_t>("discover_port");
        multicast_.reset(Socket::multicast(ip, port));
    }
    
    if (DISCOVER == state_ || HOST == state_) {
        multicast_.reset();
    }    
}

void Sockets::NetworkSystem::signal(const Signal& signal) {
    
}