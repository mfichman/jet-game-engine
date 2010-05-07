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
#include <Jet/Core/Node.hpp>
#include <Jet/Light.hpp>

namespace Jet { namespace Core {
    
//! Represents a directional or omni-directional point light.
//! @class Light
//! @brief Directional or omni-directional point light.
class Light : public Jet::Light {
public:
    inline Light(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent),
        type_(DIRECTIONAL_LIGHT),
        cast_shadows_(true) {
            
    }
    
    //! Returns the parent node.
    inline Node* parent() const {
        return parent_;
    }
    
    //! Returns the ambient color of the light.
    inline const Color& ambient_color() const {
        return ambient_color_;
    }
    
    //! Returns the diffuse color of the light.
    inline const Color& diffuse_color() const {
        return diffuse_color_;
    }
    
    //! Returns the specular color of the light.
    inline const Color& specular_color() const {
        return specular_color_;
    }
    
    //! Returns the light direction
    inline const Vector& direction() const {
        return direction_;
    }
    
    //! Sets the type of light.
    inline LightType type() const {
        return type_;
    }
    
    //! Returns true if the light casts shadows.
    bool cast_shadows() const {
        return cast_shadows_;
    }
    
    //! Sets the ambient color of the light.
    inline void ambient_color(const Color& color) {
        ambient_color_ = color;
    }
    
    //! Sets the diffuse color of the light.
    inline void diffuse_color(const Color& color) {
        diffuse_color_ = color;
    }
    
    //! Sets the specular color of the light.
    inline void specular_color(const Color& color) {
        specular_color_ = color;
    }
    
    //! Sets the light direction
    inline void direction(const Vector& direction) {
        direction_ = direction;
    }
    
    //! Sets the type of light.
    inline void type(LightType type) {
        type_ = type;
    }
    
    //! True if the light should cast shadows.
    inline void cast_shadows(bool cast_shadows) {
        cast_shadows_ = cast_shadows;
    }

private:
    Engine* engine_;
    Node* parent_;
    Color ambient_color_;
    Color diffuse_color_;
    Color specular_color_;
    Vector direction_;
    LightType type_;
    bool cast_shadows_;
};

}}