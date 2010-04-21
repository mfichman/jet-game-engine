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
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/freeglut.h>
#include <cmath>

using namespace Jet;
using namespace std;

Core::InputSystem* Core::InputSystem::system_ = 0;

Core::InputSystem::InputSystem(Engine* engine) :
    engine_(engine){

    
    system_ = this;
}

Core::InputSystem::~InputSystem() {
    system_ = 0;
}

void Core::InputSystem::on_init() {
    glutKeyboardFunc(&InputSystem::on_keyboard);
    glutKeyboardUpFunc(&InputSystem::on_keyboard_up);
    glutSpecialFunc(&InputSystem::on_special);
    glutSpecialUpFunc(&InputSystem::on_special_up);
    glutMouseFunc(&InputSystem::on_mouse);
    glutJoystickFunc(&InputSystem::on_joystick, 100);
    glutIgnoreKeyRepeat(1);
}

void Core::InputSystem::on_update() {

}

void Core::InputSystem::on_keyboard(unsigned char key, int x, int y) {
    Module* module = system_->engine_->module();
    if (module) {
        char str[2] = { key, 0 };
        real_t width = (real_t)glutGet(GLUT_WINDOW_WIDTH);
        real_t height = (real_t)glutGet(GLUT_WINDOW_HEIGHT);
        real_t xpos = 2.0f*(real_t)x/width - 1.0f;
        real_t ypos = 2.0f*(real_t)y/height - 1.0f;
        xpos = max(-1.0f, min(1.0f, xpos));
        ypos = max(-1.0f, min(1.0f, ypos));
        module->on_key_pressed(str, xpos, ypos);
    }
}

void Core::InputSystem::on_keyboard_up(unsigned char key, int x, int y) {
    Module* module = system_->engine_->module();
    if (module) {
        char str[2] = { key, 0 };
        real_t width = (real_t)glutGet(GLUT_WINDOW_WIDTH);
        real_t height = (real_t)glutGet(GLUT_WINDOW_HEIGHT);
        real_t xpos = 2.0f*(real_t)x/width - 1.0f;
        real_t ypos = 2.0f*(real_t)y/height - 1.0f;
        xpos = max(-1.0f, min(1.0f, xpos));
        ypos = max(-1.0f, min(1.0f, ypos));
        module->on_key_released(str, xpos, ypos);
    }
}

void Core::InputSystem::on_special(int key, int x, int y) {
    //Module* module = system_->engine_->module();
}

void Core::InputSystem::on_special_up(int key, int x, int y) {
    //Module* module = system_->engine_->module();
}

void Core::InputSystem::on_mouse(int button, int state, int x, int y) {
    Module* module = system_->engine_->module();
    if (module) {
        real_t width = (real_t)glutGet(GLUT_WINDOW_WIDTH);
        real_t height = (real_t)glutGet(GLUT_WINDOW_HEIGHT);
        real_t xpos = 2.0f*(real_t)x/width - 1.0f;
        real_t ypos = 2.0f*(real_t)y/height - 1.0f;
        xpos = max(-1.0f, min(1.0f, xpos));
        ypos = max(-1.0f, min(1.0f, ypos));
        if (state == GLUT_DOWN) {
            module->on_button_pressed(button, xpos, ypos);
        } else {
            module->on_button_released(button, xpos, ypos);
        }
    }
}

void Core::InputSystem::on_joystick(unsigned int button, int x, int y, int z) {
    Module* module = system_->engine_->module();
    if (module) {
        module->on_joystick(button, (real_t)x/1000.0f, (real_t)y/1000.0f, (real_t)z/1000.0f);
    }
}
