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

#include <Jet/Input/SDLInput.hpp>
#include <Jet/Core/CoreEngine.hpp>
#include <Jet/Core/CoreOverlay.hpp>
#include <Jet/Types/Point.hpp>
#include <Jet/Types/Player.hpp>
#include <SDL/SDL.h>
#include <cmath>
#include <cctype>

using namespace Jet;
using namespace std;
using namespace boost;


SDLInput::SDLInput(CoreEngine* engine) :
    engine_(engine){

    engine_->listener(this);

	for (uint32_t key = SDLK_FIRST; key != SDLK_LAST; key++) {
		key_map_.insert(make_pair(SDL_GetKeyName((SDLKey)key), (SDLKey)key));
	}
}

SDLInput::~SDLInput() {
}

void SDLInput::on_init() {
    std::cout << "Initializing input system" << std::endl;
    SDL_EnableUNICODE(true);
    //SDL_EnableKeyRepeat(200, 40);
}

void SDLInput::on_update() {
    // Poll for events, and then notify the current game module if a key
    // is pressed or a mouse butt is clicked.
    SDL_Event evt;
    while(SDL_PollEvent(&evt)) {
        switch (evt.type) {
            case SDL_QUIT: engine_->running(false); break;
            case SDL_KEYDOWN: on_key_pressed(SDL_GetKeyName(evt.key.keysym.sym)); break;
            case SDL_KEYUP: on_key_released(SDL_GetKeyName(evt.key.keysym.sym)); break;
            case SDL_MOUSEBUTTONDOWN: on_mouse_pressed(evt.button.button, evt.button.x, evt.button.y); break;
            case SDL_MOUSEBUTTONUP: on_mouse_released(evt.button.button, evt.button.x, evt.button.y); break;
			case SDL_MOUSEMOTION: on_mouse_moved(evt.motion.x, evt.motion.y); break;
        }
    }
    // Save the keyboard state using SDL_GetKeyState and SDL_GetMouseState
}

void SDLInput::on_tick() {

	int x;
	int y;
	int length;

	uint32_t delay = (uint32_t)engine_->option<float>("input_delay");
	uint32_t button = SDL_GetMouseState(&x, &y);
	uint8_t* keys = SDL_GetKeyState(&length);

	// Save the current input state and push it onto the 
	// priority queue
	local_input_state_.player_uuid = engine_->network()->current_player().uuid;
	local_input_state_.tick = engine_->tick_id() + delay;
	local_input_state_.mouse_button = button;
	local_input_state_.mouse = normalized_mouse(x, y);
	local_input_state_.key.resize(length);
	copy(keys, keys + length, local_input_state_.key.begin());

	input_state_.push(local_input_state_);

	// Upate the overlay input directly, because it doesn't need to be
	// sent across the network
	CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->screen());
	overlay->mouse_moved((float)x, (float)y);

	// For each state in the queue, process it with the previous state
	// to look for button presses, mouse movement, etc
	while (!input_state_.empty()) {
		//input_state_.top().tick <= engine_->tick_id()) {
		const InputState& state = input_state_.top();
		if (state.tick <= engine_->tick_id()) {
			input_state_.pop();
			process_state(state);
			prev_state_[state.player_uuid] = state;
		} else {
			break;
		}
	}
}

void SDLInput::process_state(const InputState& state) {
	// Get the previous state, if it exists
	const InputState& prev = prev_state_[state.player_uuid];
	
	// Get the current module, and don't do processing if no module is loaded
	Module* module = engine_->module();
	if (!module) {
		return;
	}

	// Process the keyboard input state by iterating over all the keys
	// in the keyboard and looking for differences
	for (size_t i = 0; i < min(prev.key.size(), state.key.size()); i++) {
		if (prev.key[i] != state.key[i]) {
			const string key = SDL_GetKeyName((SDLKey)i);
			if (state.key[i]) {
				module->on_key_pressed(key, state.mouse);
			} else {
				module->on_key_released(key, state.mouse);
			}
		}
	}

	// Process mouse input in a similar fashion
	for (size_t i = 1; i <= 3; i++) {
		if ((SDL_BUTTON(i) & prev.mouse_button) != (SDL_BUTTON(i) & state.mouse_button)) {
			if (SDL_BUTTON(i) & prev.mouse_button) {
				module->on_mouse_pressed(i, state.mouse);
			} else {
				module->on_mouse_released(i, state.mouse);
			}
		}
	}

	// Process the mouse position
	module->on_mouse_motion(state.mouse);
}

void SDLInput::on_key_pressed(const std::string& key) {
	// Send a notification of the key press to the overlay
    CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->focused_overlay());
    if (overlay) {
		// Convert to uppercase if SHIFT is being held by the user
		// N.B.: This doesn't work for special keys yet
		if ((key.length() == 1) && (SDL_GetModState() & KMOD_LSHIFT || SDL_GetModState() & KMOD_RSHIFT)) {
			string upper(" ");
			upper[0] = toupper(key[0]);
			overlay->key_pressed(upper);
		} else {
			overlay->key_pressed(key);
		}
    }
}

void SDLInput::on_key_released(const std::string& key) {
    CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->focused_overlay());
    if (overlay) {
        overlay->key_released(key);
    }
}

void SDLInput::on_mouse_pressed(int button, int x, int y) {

	CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->screen());
	overlay->mouse_pressed(button, (float)x, (float)y);
}

void SDLInput::on_mouse_released(int button, int x, int y) {
    /*Module* module = engine_->module();
    if (module) {
        module->on_mouse_released(button, normalized_mouse(x, y));
    }*/
	CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->screen());
	overlay->mouse_released(button, (float)x, (float)y);
}

void SDLInput::on_mouse_moved(int x, int y) {
    /*Module* module = engine_->module();
    if (module) {
        module->on_mouse_motion(normalized_mouse(x, y));
    }*/

}

void SDLInput::on_joystick(int button, int x, int y, int z) {
    /*Module* module = system_->engine_->module();
    if (module) {
        module->on_joystick(button, (float)x/1000.0f, (float)y/1000.0f, (float)z/1000.0f);
    }*/
}

Point SDLInput::normalized_mouse(int x, int y) {
    // Normalize the position of the mouse so that it is between -1 and 1.
    // (-1, -1) corresponds to the top-left corner of the screen.
    Point point;
    float width = any_cast<float>(engine_->option("display_width"));
    float height = any_cast<float>(engine_->option("display_height"));
    point.x = 2.0f*(float)x/width - 1.0f;
    point.y = 2.0f*(float)y/height - 1.0f;
    point.y = max(-1.0f, min(1.0f, point.y));
    point.x = max(-1.0f, min(1.0f, point.x));
    
    return point;
}

bool SDLInput::key_down(uint32_t uuid, const string& key) {
	const InputState& state = prev_state_[uuid];
	SDLKey keycode = key_map_[key];
	if (state.key.size() > (size_t)keycode) {
		return state.key[keycode] != 0;
	} else {
		return false;
	}
}

bool SDLInput::mouse_down(uint32_t uuid, int button) {
	const InputState& state = prev_state_[uuid];
	return (state.mouse_button & SDL_BUTTON(button)) != 0;
}

const Point& SDLInput::mouse_position(uint32_t uuid) {
	const InputState& state = prev_state_[uuid];
	return state.mouse;
}