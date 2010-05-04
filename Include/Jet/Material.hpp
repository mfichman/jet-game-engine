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
#include <Jet/Core/Engine.hpp>
#include <Jet/Types.hpp>
#include <Jet/Object.hpp>
#include <Jet/Color.hpp>

namespace Jet {

//! Specifies the color and texture properties of an object.
//! @class Material
//! @brief Specifies the color and texture properties of an object.
class Material : public Object {
public:    
    //! Returns the ambient color.
    virtual const Color& ambient_color() const=0;
    
    //! Returns the diffuse color.
    virtual const Color& diffuse_color() const=0;
    
    //! Returns the specular color.
    virtual const Color& specular_color() const=0;
    
    //! Returns the diffuse texture map.
    virtual Texture* diffuse_map() const=0;
    
    //! Returns the specular texture map.
    virtual Texture* specular_map() const=0;
    
    //! Returns the normal map name.
    virtual Texture* normal_map() const=0;
    
    //! Returns the shader.  If the default shader or fixed function pipeline
    //! is being used, this value will be null.
    virtual Shader* shader() const=0;
	
	//! REturns the specular shininess
	virtual real_t shininess() const=0;
	
	//! True if the material receives shadows.
	virtual bool receive_shadows() const=0;
	
	//! True if the material is double-sided.
	virtual bool double_sided() const=0;
    
    //! Returns the state of the resource.
    virtual ResourceState state() const=0;
	
	//! Returns the name
	virtual const std::string& name() const=0;

    //! Sets the ambient color of the material.
    //! @param color the ambient color
    virtual void ambient_color(const Color& color)=0;
    
    //! Sets the diffuse color of the material.
    //!! @param color the diffuse color
    virtual void diffuse_color(const Color& color)=0;
    
    //! Sets the specular color of the material.
    //! @param color the specular color
    virtual void specular_color(const Color& color)=0;
    
    //! Sets the diffuse texture map.
    //! @param texture the diffuse texture map
    virtual void diffuse_map(Texture* texture)=0;
    
    //! Sets the specular texture map.
    //! @param texture the specular texture map
    virtual void specular_map(Texture* texture)=0;
    
    //! Sets the normal texture map.
    //! @param texture the normal texture map
    virtual void normal_map(Texture* texture)=0;
    
    //! Sets the shader used to render this material.  If the shader is null,
    //! then the material will be rendered using the default shader or fixed
    //! function pipeline.
    //! @param shader the shader
    virtual void shader(Shader* shader)=0;
	
	//! Sets the specular shininess.
	virtual void shininess(real_t shininess)=0;
	
	//! True if the material receives shadows.
	virtual void receive_shadows(bool receive)=0;
	
	//! True if the material is double-sided.
	virtual void double_sided(bool double_sided)=0;
    
    //! Sets the diffuse texture map by name.
    //! @param name texture map name
    virtual void diffuse_map(const std::string& name)=0;
    
    //! Sets the specular texture map by name.
    //! @param name texture map name
    virtual void specular_map(const std::string& name)=0;
    
    //! Sets the normal map by name.
    //! @param name the normal map name
    virtual void normal_map(const std::string& name)=0;
    
    //! Sets the shader by name.  If the shader is null, then the material will
    //! be rendered using the default shader or fixed function pipeline.
    //! @param name the shader name
    virtual void shader(const std::string& name)=0;
    
    //! Sets the state of the resource.
    virtual void state(ResourceState state)=0;
};

}