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

#include <Jet/Core/CoreOverlay.hpp>
#include <boost/function.hpp>
#include <boost/iterator/transform_iterator.hpp>

using namespace Jet;
using namespace std;
using namespace std::tr1;
using namespace boost;

template <typename K, typename V>
V& get_value(std::pair<K, V>& p) {
	// Small transform function used for transforming the object
	// itrator.
    return p.second; 
}

CoreOverlay::~CoreOverlay() {
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

CoreOverlay* CoreOverlay::overlay(const string& name) {
    unordered_map<string, CoreOverlayPtr>::const_iterator i = overlay_.find(name);
    if (i == overlay_.end()) {
        CoreOverlayPtr overlay(new CoreOverlay(engine_, this));
        overlay_.insert(make_pair(name, overlay));
        return overlay.get();
    } else {
        return i->second.get();
    }
}

float CoreOverlay::corner_x() const {
    // Find the top-left corner of the overlay.  Depends on the horizontal
    // alignment of the overlay.
    if (AL_LEFT == horizontal_alignment_) {
        return x_;
    } else if (AL_RIGHT == horizontal_alignment_) {
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

float CoreOverlay::corner_y() const {
    // Find the top-right corner of the overlay.  Depends on the vertical
    // alignment of the overlay.
    if (AL_TOP == vertical_alignment_) {
        return y_;
    } else if (AL_BOTTOM == vertical_alignment_) {
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

    //! Returns an iterator over the objects connected to this node.
Iterator<CoreOverlayPtr> CoreOverlay::children() const {
    // Transform the unordered map iterator to a generic iterator over
	// all objects attached to this node.  This hides implementation details
	// about the node class.
	typedef unordered_map<string, CoreOverlayPtr> map_t;
	typedef boost::function<map_t::mapped_type& (map_t::value_type&)> fun_t;
	typedef transform_iterator<fun_t, map_t::const_iterator> itr_t;
	typedef pair<map_t::iterator, map_t::iterator> pair_t;

	itr_t begin = make_transform_iterator(overlay_.begin(), &get_value<const string, CoreOverlayPtr>);
	itr_t end = make_transform_iterator(overlay_.end(), &get_value<const string, CoreOverlayPtr>);

	return Iterator<CoreOverlayPtr>(begin, end);
}

void CoreOverlay::delete_overlay(CoreOverlay* overlay) {
    // Search through the has table to find the overlay
    for (unordered_map<string, CoreOverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
        if (i->second == overlay) {
            overlay_.erase(i);
            return;
        }
    }
}

//! Adds a listener to the overlay.
void CoreOverlay::listener(OverlayListener* listener) {
    if (destroyed_) {
		throw std::runtime_error("Attempted to add a listener to a node marked for deletion");
	} else {
		listener_ = listener;
	}
}

void CoreOverlay::update() {
    if (visible_) {
        if (listener_) { 
              listener_->on_update(engine_->frame_delta());
        }
        // Generate a mouse pressed event for children
        for (unordered_map<string, CoreOverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->update();
        }
    }
}

void CoreOverlay::mouse_pressed(int button, float x, float y) {
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
        for (unordered_map<string, CoreOverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->mouse_pressed(button, x, y);
        }
    }
}

void CoreOverlay::mouse_released(int button, float x, float y) {
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
        for (unordered_map<string, CoreOverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->mouse_released(button, x, y);
        }
    }
}

void CoreOverlay::mouse_moved(float x, float y) {
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
        for (unordered_map<string, CoreOverlayPtr>::iterator i = overlay_.begin(); i != overlay_.end(); i++) {
            i->second->mouse_moved(x, y);
        }
    }
}

void CoreOverlay::key_pressed(const std::string& key) {
    if (listener_ && visible_) {
        listener_->on_key_pressed(key);
    }
}

void CoreOverlay::key_released(const std::string& key) {
    if (listener_ && visible_) {
        listener_->on_key_released(key);
    }
}

void CoreOverlay::destroy() {
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
