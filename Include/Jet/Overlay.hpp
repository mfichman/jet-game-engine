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

#include <Jet/Types.hpp>
#include <Jet/Object.hpp>

namespace Jet {

//! Class to hold items that can be displayed on the screen.  Overlays can be
//! nested, and have attached text and a background.
//! @class Overlay
//! @brief Renders buttons, text, etc. on screen
class Overlay : public Object {
public:
    
    //! Returns the parent overlay.
    virtual Overlay* parent() const=0;
    
    //! Creates a new overlay that is a child of this overlay, or returns the
    //! child overlay if it already exists.
    virtual Overlay* overlay(const std::string& name)=0;
    
    //! Returns the x-coordinate of the top-left corner of the overlay.
    virtual real_t x() const=0;
    
    //! Returns the y-coordinate of the top-left corner of the overlay.
    virtual real_t y() const=0;
    
    //! Returns the width of the overlay.
    virtual real_t width() const=0;
    
    //! Returns the height of the overlay.
    virtual real_t height() const=0;
    
    //! Returns the layout mode of the overlay.  The layout mode can be either
    //! absolute (in which case, the x, y, width and height attributes are in
    //! pixels) or relative (normalized values on between 0 and 1 relative to
    //! the width and height of the containing overlay).
    virtual LayoutMode layout_mode() const=0;
    
    //! Returns the vertical alignment of the overlay.
    virtual Alignment vertical_alignment() const=0;
    
    //! Returns the horizontal alignment of the overlay.
    virtual Alignment horizontal_alingment() const=0;
    
    //! Returns the text that will be displayed by this overlay.
    virtual const std::string& text() const=0;
    
    //! Returns the text font.
    virtual Font* font() const=0;
    
    //! Returns the background texture of this overlay.
    virtual Texture* background() const=0;
    
    //! Sets the x-coordinate of the top-left corner of the overlay.
    virtual void x(real_t x)=0;
    
    //! Sets the y-coordinate of the top-left corner of the overlay.
    virtual void y(real_t y)=0;
    
    //! Sets the width of the overlay.
    virtual void width(real_t width)=0;
    
    //! Sets the height of the overlay.
    virtual void height(real_t height)=0;
    
    //! Sets the layout mode of the overlay.  The layout mode can be either
    //! absolute (in which case, the x, y, width and height attributes are in
    //! pixels) or relative (normalized values on between 0 and 1 relative to
    //! the width and height of the containing overlay).
    virtual void layout_mode(LayoutMode mode)=0;
    
    //! Sets the vertical alignment of the overlay.
    virtual void vertical_alignment(Alignment align)=0;
    
    //! Sets the horizontal alignment of the overlay.
    virtual void horizontal_alingment(Alignment align)=0;
    
    //! Sets the text attached to this overlay.
    virtual void text(const std::string& text)=0;
    
    //! Sets the text font.
    virtual void font(Font* font)=0;
    
    //! Sets the text font by name.
    //! @param name the name of the font to use
    virtual void font(const std::string& name)=0;
    
    //! Sets the background texture.
    virtual void background(Texture* texture)=0;
    
    //! Sets the background texture by name.
    //! @param name the name of the texture to use
    virtual void background(const std::string& name)=0;
};

}
