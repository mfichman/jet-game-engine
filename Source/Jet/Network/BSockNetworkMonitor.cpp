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

#include <Jet/Network/BSockNetworkMonitor.hpp>

using namespace Jet;
using namespace std;

void BSockNetworkMonitor::state_hash(uint32_t tick, uint32_t hash) {

	//if (tick > state_tick_) {
		// If the vote is greater than the current tick vote, then
		// clear the map and use that value instead.  This way, more
		// recent votes have top priority.
		state_hash_.clear();
		state_tick_ = tick;
	//}
	
	//if (tick == state_tick_) {
		// Only allow a vote to go through if the vote is equal to the
		// current vote
		std::map<uint32_t, int>::iterator i = state_hash_.find(hash);
		if (i == state_hash_.end()) {
			i = state_hash_.insert(make_pair(hash, 0)).first;
		}	
		i->second++;
	//}
}

void BSockNetworkMonitor::position(uint32_t tick, const Vector& position, const Vector& linvel) {
	if (tick == state_tick_) {
		position_ = position;
		linear_velocity_ = linvel;
	}
}

void BSockNetworkMonitor::rotation(uint32_t tick, const Quaternion& rotation, const Vector& angvel) {
	if (tick == state_tick_) {
		rotation_ = rotation;
		angular_velocity_ = angvel;
	}
}

void BSockNetworkMonitor::tick() {
	if (!state_hash_.empty()) {
		float alpha = engine_->option<float>("network_smoothness");
		float delta = (engine_->tick_id() - state_tick_) * engine_->timestep();

		if (delta > 0.5f) {
			state_hash_.clear();
			return;
		}
		Vector p = position_ + linear_velocity_ * delta;
		parent_->position(parent_->position().lerp(p, alpha));

		Quaternion r = (rotation_ + (rotation_ * Quaternion(0.0f, angular_velocity_)) * (delta * 0.5f)).unit();
		parent_->rotation(parent_->rotation().slerp(r, alpha));



		// Copy the network position over to the node
		parent_->rigid_body()->linear_velocity(linear_velocity_);
		parent_->rigid_body()->angular_velocity(angular_velocity_);

		// Count the votes for the state and choose the most popular
		map<uint32_t, int>::iterator j = state_hash_.end();
		int votes = 0;
		for (map<uint32_t, int>::iterator i = state_hash_.begin(); i != state_hash_.end(); i++) {
			if (i->second > votes) {
				j = i;
				votes = i->second;
			}
		}

		// Set the current state by using the hash
		if (j != state_hash_.end()) {
			parent_->actor()->state_hash(j->first);
		}
	}
}