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

#include <Jet/Core/Overlay.hpp>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>

using namespace Jet;
using namespace std;
using namespace std::tr1;

Core::Overlay::~Overlay() {
	if (!destroyed_) {
		// If the node hasn't been destroyed, then mark it as destroyed
		// and notify all the listeners to the node.  This case can
		// happen if the parent node is destroyed.
		destroyed_ = true;
		if (listener_) { 
            listener_->on_destroy();
        }
	}
}

Core::Overlay* Core::Overlay::overlay(const string& name) {
    unordered_map<string, OverlayPtr>::const_iterator i = overlay_.find(name);
    if (i == overlay_.end()) {
        OverlayPtr overlay(new Core::Overlay(engine_, this));
        overlay_.insert(make_pair(name, overlay));
        return overlay.get();
    } else {
        return i->second.get();
    }
}

float Core::Overlay::corner_x() const {
    // Find the top-left corner of the overlay.  Depends on the horizontal
    // alignment of the overlay.
    if (LEFT == horizontal_alignment_) {
        return x_;
    } else if (RIGHT == horizontal_alignment_) {
        if (parent_) {
            return parent_->width() - width_ + x_;
        } else {
            return engine_->option<float>("display_width") - width_ + x_;
        }
    } else {
        if (parent_) {
            return (parent_->width() - width_) / 2.0f + x_;
        } else {
            return (engine_->option<float>("display_width") - width_) / 2.0f + x_;
        }
    }
}

float Core::Overlay::corner_y() const {
    // Find the top-right corner of the overlay.  Depends on the vertical
    // alignment of the overlay.
    if (TOP == vertical_alignment_) {
        return y_;
    } else if (BOTTOM == vertical_alignment_) {
        if (parent_) {
            return parent_->height() - height_ + y_;
        } else {
            return engine_->option<float>("display_height") - height_ + y_;
        }
    } else {
        if (parent_) {
            return (parent_->height() - height_) / 2.0f + y_;
        } else {
            return (engine_->option<float>("display_height") - height_) / 2.0f + y_;
        }
    }
}

void Core::Overlay::render() {
    if (!visible_) {
        return;
    }
    
    float x = corner_x();
    float y = corner_y();
    
    // Now translate to the top-left corner of the overlay, and render
    // the text and background image
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    
    render_background();
    render_text();
    
    // Render children
    for (unordered_map<string, OverlayPtr>::const_iterator i = overlay_.begin(); i != overlay_.end(); i++) {
        i->second->render();
    }
    
    glPopMatrix();
    
}

void Core::Overlay::render_background() {
    if (background_) {
        background_->sampler(0);        
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(width_, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(width_, height_);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(0.0f, height_);    
        glEnd();
    }
}

void Core::Overlay::render_text() {
    if (!text_.empty() && font_) {
        glColor4fv(text_color_);
        glPushMatrix();
        glTranslatef(0, (float)font_->height(), 0);
        font_->render(text_);
        glPopMatrix();
    }
}

void Core::Overlay::delete_overlay(Overlay* overlay) {
    // Search through the has table to find the overlay
    for (unordered_map<string, OverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
        if (i->second == overlay) {
            overlay_.erase(i);
            return;
        }
    }
}

//! Adds a listener to the overlay.
void Core::Overlay::listener(OverlayListener* listener) {
    if (destroyed_) {
		throw std::runtime_error("Attempted to add a listener to a node marked for deletion");
	} else {
		listener_ = listener;
	}
}

void Core::Overlay::update() {
    if (visible_) {
        if (listener_) { 
              listener_->on_update(engine_->frame_delta());
        }
        // Generate a mouse pressed event for children
        for (unordered_map<string, OverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->update();
        }
    }
}

void Core::Overlay::mouse_pressed(int button, float x, float y) {
    if (visible_) {
        x -= corner_x();
        y -= corner_y();
        
        // If the point is inside the overlay's bounding box, generate an event.
        if (x >= 0 && y >= 0 && x <= width_ && y <= height_) {
            if (focusable_ && !destroyed_) {
                engine_->focused_overlay(this);
            }
            if (listener_) {
                listener_->on_mouse_pressed(button);
            }
        }
        
        // Generate a mouse pressed event for children
        for (unordered_map<string, OverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->mouse_pressed(button, x, y);
        }
    }
}

void Core::Overlay::mouse_released(int button, float x, float y) {
    if (visible_) {
        x -= corner_x();
        y -= corner_y();
        
        // If the point is inside the overlay's bounding box, generate an event.
        if (x >= 0 && y >= 0 && x <= width_ && y <= height_) {
           if (listener_) {
				listener_->on_mouse_released(button);
            }
        }
        
        // Generate a mouse pressed event for children
        for (unordered_map<string, OverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->mouse_released(button, x, y);
        }
    }
}

void Core::Overlay::mouse_moved(float x, float y) {
    if (visible_) {
        x -= corner_x();
        y -= corner_y();
        
        // If the point is inside the overlay's bounding box, generate an event.
        if (x >= 0 && y >= 0 && x <= width_ && y <= height_) {
            if (!mouse_inside_) {
                if (listener_) {
                    listener_->on_mouse_enter();
                }
                mouse_inside_ = true;
            }
        } else if (mouse_inside_) {
            if (listener_) {
                 listener_->on_mouse_exit();
            }
            mouse_inside_ = false;
        }
        
        // Generate a mouse pressed event for children
        for (unordered_map<string, OverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->mouse_moved(x, y);
        }
    }
}

void Core::Overlay::key_pressed(const std::string& key) {
    if (listener_ && visible_) {
        listener_->on_key_pressed(key);
    }
}

void Core::Overlay::key_released(const std::string& key) {
    if (listener_ && visible_) {
        listener_->on_key_released(key);
    }
}

void Core::Overlay::destroy() {
    if (destroyed_) {
        return;
    } else {
        destroyed_ = true;
        
        // Keep a reference to self.
        OverlayPtr self(this);
        
        // Remove this node from the object
        if (parent_) {
            parent_->delete_overlay(this);
        }
        
        // Motify listeners.
        if (listener_) {
            listener_->on_destroy();
        }
        
        // Remove focus
        if (engine_->focused_overlay() == this) {
            engine_->focused_overlay(0);
        }
        
        // Break cycle with listeners.
        listener_.reset();
    }
}