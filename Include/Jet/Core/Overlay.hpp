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

#include <Jet/Core/Types.hpp>
#include <Jet/Core/Texture.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Overlay.hpp>

#ifdef WINDOWS
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif

namespace Jet { namespace Core {

//! Class to hold items that can be displayed on the screen.  Overlays can be
//! nested, and have attached text and a background.
//! @class Overlay
//! @brief Renders buttons, text, etc. on screen
class Overlay : public Jet::Overlay {
public:
    
    //! Returns the parent overlay.
    inline Overlay* parent() const {
        return parent_;
    }
    
    //! Creates a new overlay that is a child of this overlay, or returns the
    //! child overlay if it already exists.
    Overlay* overlay(const std::string& name);
    
    //! Returns the x-coordinate of the top-left corner of the overlay.
    inline real_t x() const {
        return x_;
    }
    
    //! Returns the y-coordinate of the top-left corner of the overlay.
    inline real_t y() const {
        return y_;
    }
    
    //! Returns the width of the overlay.
    inline real_t width() const {
        return width_;
    }
    
    //! Returns the height of the overlay.
    inline real_t height() const {
        return height_;
    }
    
    //! Returns the layout mode of the overlay.  The layout mode can be either
    //! absolute (in which case, the x, y, width and height attributes are in
    //! pixels) or relative (normalized values on between 0 and 1 relative to
    //! the width and height of the containing overlay).
    inline LayoutMode layout_mode() const {
        return layout_mode_;
    }
    
    //! Returns the vertical alignment of the overlay.
    inline Alignment vertical_alignment() const {
        return vertical_alignment_;
    }
    
    //! Returns the horizontal alignment of the overlay.
    inline Alignment horizontal_alignment() const {
        return horizontal_alignment_;
    }
    
    //! Returns the text that will be displayed by this overlay.
    inline const std::string& text() const {
        return text_;
    }
    
    //! Returns the text font.
    inline Font* font() const {
        throw std::runtime_error("Not implemented");
    }
    
    //! Returns the background texture of this overlay.
    inline Texture* background() const {
        return background_.get();
    }
    
    //! Sets the x-coordinate of the top-left corner of the overlay.
    inline void x(real_t x) {
        x_ = x;
    }
    
    //! Sets the y-coordinate of the top-left corner of the overlay.
    inline void y(real_t y) {
        y_ = y;
    }
    
    //! Sets the width of the overlay.
    inline void width(real_t width) {
        width_ = width;
    }
    
    //! Sets the height of the overlay.
    inline void height(real_t height) {
        height_ = height;
    }
    
    //! Sets the layout mode of the overlay.  The layout mode can be either
    //! absolute (in which case, the x, y, width and height attributes are in
    //! pixels) or relative (normalized values on between 0 and 1 relative to
    //! the width and height of the containing overlay).
    inline void layout_mode(LayoutMode mode) {
        layout_mode_ = mode;
    }
    
    //! Sets the vertical alignment of the overlay.
    inline void vertical_alignment(Alignment align) {
        vertical_alignment_ = align;
    }
    
    //! Sets the horizontal alignment of the overlay.
    inline void horizontal_alignment(Alignment align) {
        horizontal_alignment_ = align;
    }
    
    //! Sets the text attached to this overlay.
    inline void text(const std::string& text) {
        text_ = text;
    }
    
    //! Sets the text font.
    inline void font(Font* font) {
        throw std::runtime_error("Not implemented");
    }
    
    //! Sets the text font by name.
    //! @param name the name of the font to use
    inline void font(const std::string& name) {
        throw std::runtime_error("Not implemented");
    }
    
    //! Sets the background texture.
    inline void background(Jet::Texture* texture) {
        background_ = static_cast<Texture*>(texture);
    }
    
    //! Sets the background texture by name.
    //! @param name the name of the texture to use
    inline void background(const std::string& name) {
        background(engine_->texture(name));
    }
    
    //! Adds a listener to the overlay.
    inline void listener(OverlayListener* listener) {
        if (destroyed_) {
			throw std::runtime_error("Attempted to add a listener to a node marked for deletion");
		} else {
			listener_.push_back(listener);
		}
    }
    
    //! Renders this overlay.
    void render();
    
    //! Destroys this overlay.
    void destroy();
    
private:
    Overlay(Engine* engine) :
        engine_(engine),
        parent_(0),
        destroyed_(false),
        x_(0.0f),
        y_(0.0f),
        width_(0.0f),
        height_(0.0f),
        layout_mode_(RELATIVE_LAYOUT),
        vertical_alignment_(TOP),
        horizontal_alignment_(LEFT) {
        
    }
    
    Overlay(Engine* engine, Overlay* parent) :
        engine_(engine),
        parent_(parent),
        destroyed_(false),
        x_(0.0f),
        y_(0.0f),
        width_(0.0f),
        height_(0.0f),
        layout_mode_(RELATIVE_LAYOUT),
        vertical_alignment_(TOP),
        horizontal_alignment_(LEFT) {
            
    }
    
    void render_background();
    void render_text();
    void delete_overlay(Overlay* overlay);
    
    Engine* engine_;
    Overlay* parent_;
    bool destroyed_;
    std::tr1::unordered_map<std::string, OverlayPtr> overlay_;
    real_t x_;
    real_t y_;
    real_t width_;
    real_t height_;
    LayoutMode layout_mode_;
    Alignment vertical_alignment_;
    Alignment horizontal_alignment_;
    std::string text_;
    //Font
    TexturePtr background_;
    std::vector<OverlayListenerPtr> listener_;
    
    friend class Engine;
};

}}
