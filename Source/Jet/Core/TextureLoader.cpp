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
 
#include <Jet/Core/TextureLoader.hpp>
#include <Jet/Texture.hpp>
#include <Jet/Engine.hpp>
#include <IL/IL.h>
#include <stdexcept>

using namespace Jet;
using namespace Jet::Core;
using namespace std;

TextureLoader::TextureLoader(Engine* engine) :
    engine_(engine) {

    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
        throw runtime_error("IL library version mismatch");
    }

    ilInit();
}

void TextureLoader::resource(const std::string& file) {
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);
    ilLoadImage(file.c_str());
    //ilGetError()

    TexturePtr texture(engine_->texture(file));
    texture->width(ilGetInteger(IL_IMAGE_WIDTH));
    texture->height(ilGetInteger(IL_IMAGE_HEIGHT));
    ilCopyPixels(0, 0, 0, texture->width(), texture->height(), 1, IL_RGBA, IL_UNSIGNED_BYTE, texture->data());
}
