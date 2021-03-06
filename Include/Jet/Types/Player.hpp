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

namespace Jet { 

//! Structure for holding information about a player
//! @class Player
//! @brief Player Contains information about a player
class Player {
public:
    //! Creates a new player with the given name
    inline Player(const std::string& name) :
        name(name),
		uuid(::rand()),
		datagram_address(0),
		datagram_port(0),
        timestamp(0.0f) {
    }
    
    //! Creates a new player
    inline Player() :
        name(""),
		uuid(::rand()),
		datagram_address(0),
		datagram_port(0),
        timestamp(0.0f) {
            
    }
    
    inline bool operator==(const Player& other) const {
        return uuid == other.uuid;
    }
    
    inline bool operator<(const Player& other) const {
        return uuid < other.uuid;
    }
    
    std::string name;
    uint32_t uuid;
	uint32_t datagram_address;
	uint16_t datagram_port;
    float timestamp;
};

}

