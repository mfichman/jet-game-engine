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
    virtual const std::string& shader_name() const=0;

    //! Returns a texture used by this material.  Materials may have more
    //! than one texture, and this function returns the texture at the given
    //! index.
    //! @param index the texture index
    virtual const std::string& texture_name(size_t index) const=0;
    
    //! Returns a cubemap used by this material.  Materials may have more
    //! than one cubemap, and this function returns the cubemap at the given
    //! index.
    //! @param index the cubemap index
    virtual const std::string& cubemap_name(size_t index) const=0;
    
    //! Returns the normal map for this object
    virtual const std::string& normal_map_name() const=0;
    
    //! Returns the environment map for this object
    virtual const std::string& environment_map_name() const=0;
    
    //! Returns the specular map for this object
    virtual const std::string& specular_map_name() const=0;
    
    //! Returns the glow map for this object
    virtual const std::string& glow_map_name() const=0;

    //! Returns the ambient color of the material.
    virtual const Color& ambient_color() const=0;

    //! Returns the diffuse color of the material.
    virtual const Color& diffuse_color() const=0;

    //! Returns the specular color of the material.
    virtual const Color& specular_color() const=0;

    //! Returns the specular power, or shininess of the material.  This
    //! controls the size of the specular highlights.
    virtual real_t shininess() const=0;

    //! Returns the reflectivity of the material.  This parameter has no effect
    //! unless a reflective shader is enabled.
    virtual real_t reflectivity() const=0;

    //! Returns the transparency of the material.  This determines the order
    //! in which the geometry is rendered.
    virtual real_t transparent() const=0;

    //! Sets the shader used by this material.  For DirectX, the shader 
    //! corresponds to <name>.fx; for GLSL it corresponds to the shaders
    //! <name>.vertex.glsl and <name>.fragment.glsl if available.
    //! @param name the name of the shader
    virtual void shader_name(const std::string& name) const=0;

    //! Sets the texture used by this material.  Materials may have more than 
    //! one texture, and this function sets the texture at the given index.
    //! @param index the index of the cubemap
    //! @param name the name of the cubemap
    virtual void texture_name(size_t index, const std::string& name) const=0;

    //! Sets the cubemap used by this material.  Materials may have more than 
    //! one cubemap, and this function sets the cubemap at the given index.
    //! @param index the index of the cubemap
    //! @param name the name of the cubemap
    virtual void cubemap_name(size_t index, const std::string& name) const=0;
    
     //! Returns the normal map for this object
    virtual void normal_map_name(const std::string& name) const=0;
    
    //! Returns the environment map for this object
    virtual void environment_map_name(const std::string& name) const=0;
    
    //! Returns the specular map for this object
    virtual void specular_map_name(const std::string& name) const=0;
    
    //! Returns the glow map for this object
    virtual void glow_map_name(const std::string& name) const=0;

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
    //! @param b true if the object is transparent
    virtual void transparent(bool b)=0;
};

}
