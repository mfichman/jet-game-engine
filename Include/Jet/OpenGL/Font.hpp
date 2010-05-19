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

#include <Jet/OpenGL/Types.hpp>
#include <Jet/Core/Engine.hpp>
#include <Jet/Font.hpp>
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include <freetype/freetype.h>
#include <vector>

namespace Jet { namespace OpenGL {

//! Class to hold a Font data for rendering.
//! @class Font
//! @brief Class to hold Font data.
class Font : public Jet::Font {
public:
    //! Creates a new font for the given engine.
    Font(Core::Engine* engine, const std::string& name);
    
    //! Destroys the font.
    virtual ~Font();
    
	//! Sets the state of the shader
	inline ResourceState state() const {
        return state_;
    }

	//! Returns the name of the texture
	inline const std::string& name() const {
        return name_;
    }
    
    //! Returns the name of the font family
    inline const std::string& face() const {
        return face_;
    }
    
    //! Returns the font size
    inline size_t height() const {
        return height_;
    }

	//! Returns the resource state of the shader
	void state(ResourceState state);
    
    //! Renders this font.  An orthographic screen
    //! space projection must be used!
    void render(const std::string& text);

private:
    void read_font_data();
    void create_bitmap(FT_Face face, unsigned char letter);
    
    Core::Engine* engine_;
    std::string name_;
    std::string face_;
    size_t height_;
    ResourceState state_;
    std::vector<GLuint> texture_;
    GLuint list_;
};

}}
