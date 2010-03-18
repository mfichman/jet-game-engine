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

#include <Jet/Jet.hpp>
#include <Jet/SceneComponent.hpp>
#include <string>

namespace Jet {

//! Displays a rectangular quad using a material.  This can be used for 
//! various effects, including billboarding if the proper vertex shader is
//! used.  All quads with the same material are automatically grouped into
//! batches for maximum performance.
//! @class Quad
//! @brief Displays a solid quad.
class JETAPI Quad : public SceneComponent {
public:
    //! Destroys the quad.
    virtual ~Quad() {}

    //! Returns the name of the material used to render this quad.
    const std::string& get_material() const { return material; }

    //! Returns the width of this quad in world coordinates.
    real_t get_width() const { return width; }

    //! Returns the height of this quad in world coordinates.
    real_t get_height() const { return height; }

    //! Sets the material used to render this quad.  See the documentation 
    //! regarding materials for more information.
    void set_material(const std::string& s) { material = s; }

    //! Sets the width of this quad in world coordinates.
    //! @param f the new width of the quad
    void set_width(real_t f) { width = f; }

    //! Sets the height of this quad in world coordinates.
    //! @param f the new height of this quad
    void set_height(real_t f) { height = f; }

private:
    Quad(SceneNode* parent, const std::string& name);
    friend class SceneNode;

    std::string material;
    real_t width;
    real_t height;
};

}
