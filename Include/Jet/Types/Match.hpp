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
#include <Jet/Types/Address.hpp>

namespace Jet {

//! Holds information about a multiplayer game.
//! @class Match
//! @brief Holds information about a multiplayer game.
class Match {
public:
    //! Creates a new game with the given name.
    inline Match(const std::string& name) :
        name(name),
        uuid(rand()),
        timestamp(0.0f) {
        
    }
    
    //! Creates a new game
    inline Match() :
        name("Unknown"),
        uuid(rand()),
        timestamp(0.0f) {
    }
    
    inline bool operator==(const Match& other) const {
        return uuid == other.uuid;
    }
    
    inline bool operator<(const Match& other) const {
        return uuid < other.uuid;
    }
    
    std::string name;
    Address stream_address;
	Address datagram_address;
    uint32_t uuid;
    float timestamp;
};

}
