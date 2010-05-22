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
#include <Jet/Core/CoreOverlay.hpp> // TODO HACK HACK HACK
#include <Jet/Types/Point.hpp>
#include <SDL/SDL.h>
#include <cmath>
#include <cctype>

using namespace Jet;
using namespace std;
using namespace boost;


SDLInput::SDLInput(CoreEngine* engine) :
    engine_(engine){

    engine_->listener(this);
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
    
}

void SDLInput::on_tick() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->screen());
	overlay->mouse_moved((float)x, (float)y);
}

void SDLInput::on_key_pressed(const std::string& key) {
    Module* module = engine_->module();
    if (module) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        module->on_key_pressed(key, normalized_mouse(x, y));
    }


    CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->focused_overlay());
    if (overlay) {
		if ((key.length() == 1) && (SDL_GetModState() & KMOD_LSHIFT || SDL_GetModState() & KMOD_RSHIFT)) {
			//if (isalpha(key[0])) {
				string upper(" ");
				upper[0] = toupper(key[0]);
				overlay->key_pressed(upper);
			//}
		} else {
			overlay->key_pressed(key);
		}
    }
}

void SDLInput::on_key_released(const std::string& key) {
    Module* module = engine_->module();
    if (module) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        module->on_key_released(key, normalized_mouse(x, y));
    }
    CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->focused_overlay());
    if (overlay) {
        overlay->key_released(key);
    }
}

void SDLInput::on_mouse_pressed(int button, int x, int y) {
    Module* module = engine_->module();
    if (module) {
        module->on_mouse_pressed(button, normalized_mouse(x, y));
    }
	CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->screen());
	overlay->mouse_pressed(button, (float)x, (float)y);
}

void SDLInput::on_mouse_released(int button, int x, int y) {
    Module* module = engine_->module();
    if (module) {
        module->on_mouse_released(button, normalized_mouse(x, y));
    }
	CoreOverlay* overlay = static_cast<CoreOverlay*>(engine_->screen());
	overlay->mouse_released(button, (float)x, (float)y);
}

void SDLInput::on_mouse_moved(int x, int y) {
    Module* module = engine_->module();
    if (module) {
        module->on_mouse_motion(normalized_mouse(x, y));
    }

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
