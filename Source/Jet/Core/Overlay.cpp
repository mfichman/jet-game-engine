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
#include <GL/gl.h>

using namespace Jet;
using namespace std;
using namespace std::tr1;

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

void Core::Overlay::render() {
    float x;
    float y;
    
    // Find the top-left corner of the overlay.  Depends on the horizontal
    // alignment of the overlay.
    if (LEFT == horizontal_alignment_) {
        x = x_;
    } else if (RIGHT == horizontal_alignment_) {
        if (parent_) {
            x = parent_->width() - width_ + x_;
        } else {
            x = engine_->option<float>("display_width") - width_ + x_;
        }
    } else {
        if (parent_) {
            x = (parent_->width() - width_) / 2.0f + x_;
        } else {
            x = (engine_->option<float>("display_width") - width_) / 2.0f + x_;
        }
    }
    
    // Find the top-right corner of the overlay.  Depends on the vertical
    // alignment of the overlay.
    if (TOP == vertical_alignment_) {
        y = y_;
    } else if (BOTTOM == vertical_alignment_) {
        if (parent_) {
            y = parent_->height() - height_ + y_;
        } else {
            y = engine_->option<float>("display_height") - height_ + y_;
        }
    } else {
        if (parent_) {
            y = (parent_->height() - height_) / 2.0f + y_;
        } else {
            y = (engine_->option<float>("display_height") - height_) / 2.0f + y_;
        }
    }
    
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
        glEnable(GL_TEXTURE_2D);
        
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

        glDisable(GL_TEXTURE_2D);
    }
}

void Core::Overlay::render_text() {
    
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
        for (vector<OverlayListenerPtr>::iterator i = listener_.begin(); i != listener_.end(); i++) {
            (*i)->on_destroy();
        }
        
        // Break cycle with listeners.
        listener_.clear();
    }
}