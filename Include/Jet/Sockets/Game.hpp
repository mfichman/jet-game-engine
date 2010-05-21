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

namespace Jet { namespace Sockets {

//! Structure for holding information about a multiplayer game.
//! @class Game
//! @brief Game value
class Game {
public:
    //! Creates a new game
    Game() :
        server_port(0),
        time(0.0f) {
    }
    
    bool operator==(const Game& other) const {
        return name == other.name && server_port == other.server_port && server_address == other.server_address;
    }
    
    bool operator<(const Game& other) const {
        if (name != other.name) {
            return name < other.name;
        }
        if (server_port != other.server_port) {
            return server_port < other.server_port;
        }
        if (server_address != other.server_address) {
            return server_address < other.server_address;
        }
        return false;
    }
    
    std::string name;
    uint16_t server_port;
    std::string server_address;
    float time;
};

}}

