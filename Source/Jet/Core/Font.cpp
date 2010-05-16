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

#include <Jet/Core/Font.hpp>
#include <Jet/Core/engine.hpp>
#include <freetype/ftglyph.h>
#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>

using namespace Jet;
using namespace std;

inline int next_p2(int a) {
	int rval=1;
	while(rval < a) {
        rval *= 2;
    }
	return rval;
}

Core::Font::Font(Engine* engine, const std::string& name) :
    engine_(engine),
    name_(name),
    state_(UNLOADED),
    list_(0) {
        
    size_t pos = name.find("#");
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid font name, don't forget to append #size");
    }
    face_ = name.substr(0, pos);
    height_ = boost::lexical_cast<size_t>(name.substr(pos+1, name.size()-pos-1));
}

Core::Font::~Font() {
    state(UNLOADED);
}

void Core::Font::state(ResourceState state) {
    if (state_ == state) {
        return;
    }
    
    if (LOADED == state) {
        read_font_data();
    }
    
    if (LOADED == state_) {
        if (list_ && !texture_.empty()) {
            glDeleteLists(list_, texture_.size());
            glDeleteTextures(128, &texture_[0]);
            texture_.resize(0);
        }
    }
    
    state_ = state;
}

void Core::Font::read_font_data() {
    // This routine taken from nehe.gamedev.net
    const std::string path = engine_->resource_path(face_);
    texture_.resize(128);
    
    // Create and initialize a new FreeType font library handle.
    FT_Library library;
    if (FT_Init_FreeType(&library)) {
        throw runtime_error("Could not initialize font library");
    }
    
    // Create and initialize a new font face.
    FT_Face face;
    if (FT_New_Face(library, path.c_str(), 0, &face)) {
        throw runtime_error("Could not initialize font");
    }
    
    // FreeType measures font size in 1/64ths of pixels.  So we multiply the
    // height by 64 to get the right size
    FT_Set_Char_Size(face, height_*64, height_*64, 96, 96);
    
    // Generate GL display lists and textures
    glGenTextures(128, &texture_[0]);
    list_ = glGenLists(texture_.size());
    for (unsigned char i = 0; i < 128; i++) {
        create_bitmap(face, i);
    }
    
    // Release the face and font library
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

void Core::Font::create_bitmap(FT_Face face, unsigned char ch) {
    // Load the glyph for the current character
    if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
        throw runtime_error("Could not load font glyph");
    }
    
    // Move the face's glyph into a glyph object
    FT_Glyph glyph;
    if (FT_Get_Glyph(face->glyph, &glyph)) {
        throw runtime_error("Could not get glyph object");
    }
    
    // Convert to a bitmap so that it can be stored in a texture
    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
    FT_Bitmap bitmap = bitmap_glyph->bitmap;
    
    // Get the width and height of the bitmap data
    int width = next_p2(bitmap.width);
    int height = next_p2(bitmap.rows);
    
    // Allocate memory fo the texture data
    std::vector<unsigned char> bitmap_data(2*width*height);
    
    // Fill the data for the bitmap.  This is a two-channel bitmap,
    // with luminosity and alpha (no need for separate RGB components)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int value;
            if (i >= bitmap.width || j >= bitmap.rows) {
                value = 0;
            } else {
                value = bitmap.buffer[i + bitmap.width*j];
            }
            bitmap_data[2*(i+j*width)] = value; // Luminosity
            bitmap_data[2*(i+j*width)+1] = value; // Alpha
        }
    }
    
    // Now initialize the texture.
    glBindTexture(GL_TEXTURE_2D, texture_[ch]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, &bitmap_data[0]);

    // Start a new display list.
    glNewList(list_+ch, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, texture_[ch]);
    glPushMatrix();
    
    // Adjust for left spacing and descent of the glyph.
    glTranslatef((float)bitmap_glyph->left, 0, 0);
    glTranslatef(0, (float)(bitmap.rows-bitmap_glyph->top), 0);
    
    // Compensate for blank space surrounding the character.
    float x = (float)bitmap.width / (float)width;
    float y = (float)bitmap.rows / (float)height;

    // Draw the quad for the glyph.
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0); glVertex2f(0, -(float)bitmap.rows);
    glTexCoord2d(0, y); glVertex2f(0, 0);
    glTexCoord2d(x, y); glVertex2f((float)bitmap.width, 0);
    glTexCoord2d(x, 0); glVertex2f((float)bitmap.width, -(float)bitmap.rows);
    glEnd();
    glPopMatrix();
    
    // Translate by an amount equal to the width of the character.
    // This puts the cursor in the right position to render
    // the next character.
    glTranslatef((float)face->glyph->advance.x/64, 0, 0);
    
    glEndList();
}

void Core::Font::render(const std::string& text) {
    state(LOADED);

    // Push some parameters to enable proper texturing
    glListBase(list_);
    
    // Render all the lists necessary at once using the string
    // directly (very efficient!!)
    glPushMatrix();
    glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
    glPopMatrix();
    
    // Pop any old attributes
    glListBase(0);
}