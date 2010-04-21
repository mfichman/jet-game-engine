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
#include <SDL/SDL.h>
#include <cmath>

using namespace Jet;
using namespace std;
using namespace boost;


Core::InputSystem::InputSystem(Engine* engine) :
    engine_(engine){

}

Core::InputSystem::~InputSystem() {
}

void Core::InputSystem::on_init() {
    SDL_EnableUNICODE(true);
}

void Core::InputSystem::on_update() {
    SDL_EnableUNICODE(true);
    SDL_Event evt;
    while(SDL_PollEvent(&evt)) {
        switch (evt.type) {
            case SDL_QUIT: engine_->running(false); break;
            case SDL_KEYDOWN: on_keyboard(evt.key.keysym.unicode & 0x7F); break;
            case SDL_KEYUP: on_keyboard_up(evt.key.keysym.unicode & 0x7F); break;
            case SDL_MOUSEBUTTONDOWN: on_mouse(evt.button.button, evt.button.x, evt.button.y); break;
            case SDL_MOUSEBUTTONUP: on_mouse_up(evt.button.button, evt.button.x, evt.button.y); break;
        }
    }

}

void Core::InputSystem::on_keyboard(char key) {
    Module* module = engine_->module();
    if (module) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        real_t width = any_cast<real_t>(engine_->option("display_width"));
        real_t height = any_cast<real_t>(engine_->option("display_height"));
        real_t xpos = 2.0f*(real_t)x/width - 1.0f;
        real_t ypos = 2.0f*(real_t)y/height - 1.0f;
        xpos = max(-1.0f, min(1.0f, xpos));
        ypos = max(-1.0f, min(1.0f, ypos));
        char str[2] = { key, 0 };
        module->on_key_pressed(str, xpos, ypos);
    }
}

void Core::InputSystem::on_keyboard_up(char key) {
    Module* module = engine_->module();
    if (module) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        real_t width = any_cast<real_t>(engine_->option("display_width"));
        real_t height = any_cast<real_t>(engine_->option("display_height"));
        real_t xpos = 2.0f*(real_t)x/width - 1.0f;
        real_t ypos = 2.0f*(real_t)y/height - 1.0f;
        xpos = max(-1.0f, min(1.0f, xpos));
        ypos = max(-1.0f, min(1.0f, ypos));
        char str[2] = { key, 0 };
        module->on_key_released(str, xpos, ypos);
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
        real_t width = any_cast<real_t>(engine_->option("display_width"));
        real_t height = any_cast<real_t>(engine_->option("display_height"));
        real_t xpos = 2.0f*(real_t)x/width - 1.0f;
        real_t ypos = 2.0f*(real_t)y/height - 1.0f;
        xpos = max(-1.0f, min(1.0f, xpos));
        ypos = max(-1.0f, min(1.0f, ypos));
        module->on_button_pressed(button, xpos, ypos);
    }
}

void Core::InputSystem::on_mouse_up(int button, int x, int y) {
    Module* module = engine_->module();
    if (module) {
        real_t width = any_cast<real_t>(engine_->option("display_width"));
        real_t height = any_cast<real_t>(engine_->option("display_height"));
        real_t xpos = 2.0f*(real_t)x/width - 1.0f;
        real_t ypos = 2.0f*(real_t)y/height - 1.0f;
        xpos = max(-1.0f, min(1.0f, xpos));
        ypos = max(-1.0f, min(1.0f, ypos));
        module->on_button_released(button, xpos, ypos);
    }
}

void Core::InputSystem::on_joystick(int button, int x, int y, int z) {
    /*Module* module = system_->engine_->module();
    if (module) {
        module->on_joystick(button, (real_t)x/1000.0f, (real_t)y/1000.0f, (real_t)z/1000.0f);
    }*/
}
