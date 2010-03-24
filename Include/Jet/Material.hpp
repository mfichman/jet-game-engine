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
#include <Jet/Color.hpp>
#include <Jet/Object.hpp>
#include <string>
#include <vector>

namespace Jet {

//! Set of parameters used for rendering objects.
//! @class Material
//! @brief Parameters used for rendering objects.
class Material : public Object {
public:
    
    //! Destructor
    virtual ~Material() {}
    
    //! Returns the shader used by this material.
    virtual const std::string& get_shader() const=0;
    
    //! Returns the normal map for this object
    virtual const std::string& get_normal_map() const=0;
    
    //! Returns the environment map for this object
    virtual const std::string& get_environment_map() const=0;
    
    //! Returns the specular map for this object
    virtual const std::string& get_specular_map() const=0;
    
    //! Returns the glow map for this object
    virtual const std::string& get_glow_map() const=0;

    //! Returns the ambient color of the material.
    virtual const Color& get_ambient_color() const=0;

    //! Returns the diffuse color of the material.
    virtual const Color& get_diffuse_color() const=0;

    //! Returns the specular color of the material.
    virtual const Color& get_specular_color() const=0;

    //! Returns the specular power, or shininess of the material.  This
    //! controls the size of the specular highlights.
    virtual real_t get_shininess() const=0;

    //! Returns the reflectivity of the material.  This parameter has no effect
    //! unless a reflective shader is enabled.
    virtual real_t get_reflectivity() const=0;

    //! Returns the transparency of the material.  This determines the order
    //! in which the geometry is rendered.
    virtual real_t get_transparency() const=0;

    //! Returns the glow power of this material.
    virtual real_t get_glow() const=0;

    //! Sets the shader used by this material.  For DirectX, the shader 
    //! corresponds to <name>.fx; for GLSL it corresponds to the shaders
    //! <name>.vertex.glsl and <name>.fragment.glsl if available.
    //! @param name the name of the shader
    virtual void shader(const std::string& name) const=0;
    
     //! Returns the normal map for this object
    virtual void normal_map(const std::string& name) const=0;
    
    //! Returns the environment map for this object
    virtual void environment_map(const std::string& name) const=0;
    
    //! Returns the specular map for this object
    virtual void specular_map(const std::string& name) const=0;
    
    //! Returns the glow map for this object
    virtual void glow_map(const std::string& name) const=0;

    //! Sets the ambient color used by this material. 
    //! @param c the color
    virtual void ambient_color(const Color& c)=0;

    //! Sets the diffuse color used by this material.
    //! @param c the color
    virtual void diffuse_color(const Color& c)=0;

    //! Sets the specular color used by this material.
    //! @param c the color
    virtual void specular_color(const Color& c)=0;
    
    //! Sets the specular power used by this material.
    //! @param f the specular power
    virtual void shininess(real_t f)=0;

    //! Sets the reflectivity of the material.  This parameter has no effect
    //! unless a reflective shader is enabled.
    //! @param f the reflectivity parameter (between 0 and 1), where 1 is the
    //! most reflective
    virtual void reflectivity(real_t f)=0;

    //! Enables/disables the transparency of the material.  This determines the 
    //! order in which the geometry is rendered.
    //! @param f the transparency of the material
    virtual void transparency(real_t f)=0;

    //! Sets the glow of the material.  This determines how much the material
    //! will glow (i.e., scaling of the glow map).
    virtual void glow(real_t f)=0;

protected:
    //! clones this material
    virtual Material* clone()=0;
};

}
