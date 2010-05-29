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
#include <Jet/Scene/NetworkMonitor.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Core/CoreNode.hpp>
#include <Jet/Utility.hpp>
#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <map>

namespace Jet {

//! This class monitors the node for state changes, and synchronizes those
//! changes with the server/peer.
//! @class NetworkMonitor
//! @brief Interface for monitoring network state change
class BSockNetworkMonitor : public NetworkMonitor {
public:
	inline BSockNetworkMonitor(CoreEngine* engine, CoreNode* node) :
		engine_(engine),
		parent_(node),
		hash_(Jet::hash(node->name())),
		state_tick_(0),
		player_uuid_(0) {
	}

    //! Returns the parent node.
	inline CoreNode* parent() const {
		return parent_;
	}

	//! Returns the UUID of the owner of the node
	inline uint32_t player_uuid() const {
		return player_uuid_;
	}

	//! Returns the network ID of the node.
	inline uint32_t hash() const {
		return hash_;
	}

	//! Destroys the network monitor
	inline void destroy() {
		parent_ = 0;
	}

	//! Sets the player UUID
	inline void player_uuid(uint32_t uuid) {
		player_uuid_ = uuid;
	}

	//! Adds a vote for the hash.  The state of the node at the end 
	//! of a network update cycle will be the most common, most recent
	//! state given by any node.
	void state_hash(uint32_t tick, uint32_t hash);

	//! Adds to the position average for this network monitor.
	void position(uint32_t tick, const Vector& position, const Vector& linvel);

	//! Adds to the rotation average for this network monitor.
	void rotation(uint32_t tick, const Quaternion& rotation, const Vector& angvel);

	//! Called when a tick happens to update the state of the node.
	void tick();

private:
	CoreEngine* engine_;
	CoreNode* parent_;
	uint32_t hash_;
	uint32_t state_tick_;
	std::map<uint32_t, int> state_hash_;
	Vector linear_velocity_;
	Vector angular_velocity_;
	Vector position_;
	Quaternion rotation_;
	uint32_t player_uuid_;
};

}