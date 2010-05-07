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

#include <Jet/Core/InputSystem.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Point.hpp>
#include <SDL/SDL.h>
#include <cmath>

using namespace Jet;
using namespace std;
using namespace boost;


Core::InputSystem::InputSystem(Engine* engine) :
    engine_(engine){

    engine_->listener(this);
}

Core::InputSystem::~InputSystem() {
}

void Core::InputSystem::on_init() {
    std::cout << "Initializing input system" << std::endl;
    SDL_EnableUNICODE(true);
}

void Core::InputSystem::on_tick() {
    // Poll for events, and then notify the current game module if a key
    // is pressed or a mouse butt is clicked.
    SDL_Event evt;
    while(SDL_PollEvent(&evt)) {
        switch (evt.type) {
            case SDL_QUIT: engine_->running(false); break;
            case SDL_KEYDOWN: on_keyboard(SDL_GetKeyName(evt.key.keysym.sym)); break;
            case SDL_KEYUP: on_keyboard_up(SDL_GetKeyName(evt.key.keysym.sym)); break;
            case SDL_MOUSEBUTTONDOWN: on_mouse(evt.button.button, evt.button.x, evt.button.y); break;
            case SDL_MOUSEBUTTONUP: on_mouse_up(evt.button.button, evt.button.x, evt.button.y); break;
        }
    }
}

void Core::InputSystem::on_keyboard(const std::string& key) {
    Module* module = engine_->module();
    if (module) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        module->on_key_pressed(key, normalized_mouse(x, y));
    }
}

void Core::InputSystem::on_keyboard_up(const std::string& key) {
    Module* module = engine_->module();
    if (module) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        module->on_key_released(key, normalized_mouse(x, y));
    }
}

void Core::InputSystem::on_special(int key, int x, int y) {
    //Module* module = system_->engine_->module();
}

void Core::InputSystem::on_special_up(int key, int x, int y) {
    //Module* module = system_->engine_->module();
}

void Core::InputSystem::on_mouse(int button, int x, int y) {
    Module* module = engine_->module();
    if (module) {
        module->on_button_pressed(button, normalized_mouse(x, y));
    }
}

void Core::InputSystem::on_mouse_up(int button, int x, int y) {
    Module* module = engine_->module();
    if (module) {
        module->on_button_released(button, normalized_mouse(x, y));
    }
}

void Core::InputSystem::on_joystick(int button, int x, int y, int z) {
    /*Module* module = system_->engine_->module();
    if (module) {
        module->on_joystick(button, (float)x/1000.0f, (float)y/1000.0f, (float)z/1000.0f);
    }*/
}

Point Core::InputSystem::normalized_mouse(int x, int y) {
    // Normalize the position of the mouse so that it is between -1 and 1.
    // (-1, -1) corresponds to the top-left corner of the screen.
    Point point;
    float width = any_cast<float>(engine_->option("display_width"));
    float height = any_cast<float>(engine_->option("display_height"));
    point.x = 2.0f*(float)x/width - 1.0f;
    point.y = 2.0f*(float)y/height - 1.0f;
    point.y = max(-1.0f, min(1.0f, point.x));
    point.x = max(-1.0f, min(1.0f, point.y));
    
    return point;
}
