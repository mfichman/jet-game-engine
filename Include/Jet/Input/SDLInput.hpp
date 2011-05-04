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

#include <Jet/Input/SDLTypes.hpp>
#include <Jet/Types/InputState.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Input.hpp>
#include <queue>
#ifdef WINDOWS
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif

namespace Jet {

//! Physics system.  Animates physical objects and performs collision
//! detection.
//! @class Input
//! @brief Rigid body physics engine
class SDLInput : public Input, public EngineListener {
public:
    //! Creates a new input system.
    SDLInput(CoreEngine* engine);
    
    //! Destructor.
    ~SDLInput();

	//! Returns the current input state for the local player
	inline const InputState& input_state() const {
		return local_input_state_;
	}

	//! Adds an input state to the input queue 
	inline void input_state(const InputState& state) {
		if (state.tick >= engine_->tick_id()) {
			input_state_.push(state);
		}
	}

	//! Returns true if the key is down for the given player
	bool key_down(uint32_t uuid, const std::string& key);

	//! Returns true if the mouse is down for the given key and player
	bool mouse_down(uint32_t uuid, int button);

	//! Returns the mouse position
	const Point& mouse_position(uint32_t uuid);

private:
    void on_tick();
    void on_init();
    void on_update();
    void on_render() {}
    
	void on_key_pressed(const std::string& key);
    void on_key_released(const std::string& key);
    void on_mouse_pressed(int button, int x, int y);
    void on_mouse_released(int button, int x, int y);
	void on_mouse_moved(int x, int y);
    void on_joystick(int button, int x, int y, int z);

	void process_state(const InputState& state);
    
    Point normalized_mouse(int x, int y);
    
    CoreEngine* engine_;
	InputState local_input_state_;
	std::priority_queue<InputState> input_state_;
	std::map<uint32_t, InputState> prev_state_;
	std::tr1::unordered_map<std::string, SDLKey> key_map_;
};

}
