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
 
#include <Jet/Core/CoreActor.hpp>
 
using namespace Jet;
using namespace std;
 

void CoreActor::state(const std::string& state) {
    if (state_ == state) {
        // No state change (idempotent state)
        return;
    }
    
    // Look up the new state and make sure it exists
    map<string, ActorStatePtr>::iterator i = actor_state_.find(state);
    if (i == actor_state_.end()) {
        throw runtime_error("Could not find state: " + state);
    }
    
    // Now switch states to the new state
    if (current_state_) {
        current_state_->on_state_exit();
    }
    state_ = state;
    current_state_ = i->second.get();
    current_state_->on_state_enter();
}

void CoreActor::state_hash(uint32_t hash) {

	// Search through all the states for a hash that matches
	for (map<string, ActorStatePtr>::iterator i = actor_state_.begin(); i != actor_state_.end(); i++) {
		uint32_t local = Jet::hash(i->first);
		if (Jet::hash(i->first) == hash) {
			state(i->first);
		}
	}
}