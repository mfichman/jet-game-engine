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
#include <Jet/Core/Font.hpp>
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
    //! Creates a new overlay for the given engine.  This overlay will have no
    //! parent, so it should be the root.
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
        horizontal_alignment_(LEFT),
        text_color_(1.0f, 1.0f, 1.0f, 1.0f),
        visible_(true),
        focusable_(false) {
        
    }
    
    //! Creates a new overlay for the engine, with the given parent.
    Overlay(Engine* engine, Overlay* parent) :
        engine_(engine),
        parent_(parent),
        destroyed_(false),
        x_(0.0f),
        y_(0.0f),
        width_(parent->width_),
        height_(parent->height_),
        layout_mode_(RELATIVE_LAYOUT),
        vertical_alignment_(TOP),
        horizontal_alignment_(LEFT),
        text_color_(1.0f, 1.0f, 1.0f, 1.0f),
        mouse_inside_(false),
        visible_(true),
        focusable_(false) {
            
    }

	//! Destructor
	virtual ~Overlay();
    
    //! Returns the parent overlay.
    inline Overlay* parent() const {
        return parent_;
    }
    
    //! Creates a new overlay that is a child of this overlay, or returns the
    //! child overlay if it already exists.
    Overlay* overlay(const std::string& name);
    
    //! Returns true if the overlay is visible.
    inline bool visible() const {
        return visible_;
    }
    
    //! Returns true if the overlay is focusable
    inline bool focusable() const {
        return focusable_;
    }
    
    //! Returns the x-coordinate of the top-left corner of the overlay.
    inline float x() const {
        return x_;
    }
    
    //! Returns the y-coordinate of the top-left corner of the overlay.
    inline float y() const {
        return y_;
    }

	//! Returns the screen x-coordinate
	float corner_x() const;

	//! Returns the screen y-coordinate
	float corner_y() const;
    
    //! Returns the width of the overlay.
    inline float width() const {
        return width_;
    }
    
    //! Returns the height of the overlay.
    inline float height() const {
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
    
    //! Returns the text color
    inline const Color& text_color() const {
        return text_color_;
    }
    
    //! Returns the text font.
    inline Font* font() const {
        throw std::runtime_error("Not implemented");
    }
    
    //! Returns the background texture of this overlay.
    inline Texture* background() const {
        return background_.get();
    }
    
    //! Makes this overlay visible/invisible.
    inline void visible(bool visible) {
        visible_ = visible;
    }
    
    //! Makes this overlay is focusable.
    inline void focusable(bool focusable) {
        focusable_ = focusable;
		engine_->focused_overlay(this);
    }
    
    //! Sets the x-coordinate of the top-left corner of the overlay.
    inline void x(float x) {
        x_ = x;
    }
    
    //! Sets the y-coordinate of the top-left corner of the overlay.
    inline void y(float y) {
        y_ = y;
    }
    
    //! Sets the width of the overlay.
    inline void width(float width) {
        width_ = width;
    }
    
    //! Sets the height of the overlay.
    inline void height(float height) {
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
    
    //! Sets the text color
    inline void text_color(const Color& color) {
        text_color_ = color;
    }
    
    //! Sets the text font.
    inline void font(Jet::Font* font) {
        font_ = static_cast<Font*>(font);
    }
    
    //! Sets the text font by name.
    //! @param name the name of the font to use
    inline void font(const std::string& name) {
        font(engine_->font(name));
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
    void listener(OverlayListener* listener);
    
    //! Renders this overlay.
    void render();
    
    //! Destroys this overlay.
    void destroy();
    
    //! Updates this overlay.
    void update();
        
    //! Mouse event.
    void mouse_pressed(int button, float x, float y);
    
    //! Mouse event
    void mouse_released(int button, float x, float y);
    
    //! Mouse event
    void mouse_moved(float x, float y);
    
    //! Key event
    void key_pressed(const std::string& key);

    //! Key event
    void key_released(const std::string& key);
    
private:    
    void render_background();
    void render_text();
    void delete_overlay(Overlay* overlay);
    
    Engine* engine_;
    Overlay* parent_;
    bool destroyed_;
    std::tr1::unordered_map<std::string, OverlayPtr> overlay_;
    float x_;
    float y_;
    float width_;
    float height_;
    LayoutMode layout_mode_;
    Alignment vertical_alignment_;
    Alignment horizontal_alignment_;
    std::string text_;
    Color text_color_;
    TexturePtr background_;
    FontPtr font_;
    bool mouse_inside_;
    bool visible_;
    bool focusable_;
    OverlayListenerPtr listener_;
};

}}
