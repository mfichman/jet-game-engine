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
#include <Jet/Types/Color.hpp>
#include <Jet/Types/Vector.hpp>

namespace Jet {
    
//! Represents a directional or omni-directional point light.
//! @class Light
//! @brief Directional or omni-directional point light.
class Light : public Object {
public:    
    //! Returns the parent node.
    virtual Node* parent() const=0;
    
    //! Returns the ambient color of the light.
    virtual const Color& ambient_color() const=0;
    
    //! Returns the diffuse color of the light.
    virtual const Color& diffuse_color() const=0;
    
    //! Returns the specular color of the light.
    virtual const Color& specular_color() const=0;
    
    //! Returns the light direction
    virtual const Vector& direction() const=0;
    
    //! Sets the type of light.
    virtual LightType type() const=0;
    
    //! Returns true if the light casts shadows.
    virtual bool cast_shadows() const=0;
    
    //! Sets the ambient color of the light.
    virtual void ambient_color(const Color& color)=0;
    
    //! Sets the diffuse color of the light.
    virtual void diffuse_color(const Color& color)=0;
    
    //! Sets the specular color of the light.
    virtual void specular_color(const Color& color)=0;
    
    //! Sets the light direction
    virtual void direction(const Vector& direction)=0;
    
    //! Sets the type of light.
    virtual void type(LightType type)=0;
    
    //! True if the light should cast shadows.  Note that current versions of
    //! the engine only allow one light to cast shadows.
    virtual void cast_shadows(bool cast_shadows)=0;
};

}