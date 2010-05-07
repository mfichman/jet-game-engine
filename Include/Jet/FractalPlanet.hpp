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

//! Renders a planet that becomes more detailed as the camera approaches it.
//! @class FractalPlanet
//! Generates a planet.
class FractalPlanet : public Object {
public:
    //! Returns the parent of this planet.
    virtual Node* parent() const=0;
    
    //! Returns the seed used to generate the planet.
    virtual size_t seed() const=0;
    
    //! Returns the roughness of the planet.  0 is the smoothest, and 1 is the
    //! roughest.
    virtual float roughness() const=0;
    
    //! Returns the orbit material.
    virtual Material* orbit_material() const=0;
    
    //! Returns the ground texture.
    virtual Material* ground_material() const=0;
    
    //! Returns the number of rings used to generate the planet sphere.
    virtual size_t  ring_count() const=0;
    
    //! Returns the number of divisions used to generate the planet sphere.
    //! This is equal to the number of lines of longitude.
    virtual size_t  division_count() const=0;
    
    //! Returns the number of detail levels.
    virtual size_t detail_level_count() const=0;
    
    //! Sets the seed used to generate the planet.
    //! @param seed the random seed.
    virtual void seed(size_t seed)=0;

    //! Sets the roughness of the planet.  0 is the smoothest, and 1 is the
    //! roughest.
    //! @param roughness the roughness parameter
    virtual void roughness(float roughness)=0;
    
    //! Sets the orbit material.  This is the material visible from orbit.
    //! @param material the material
    virtual void orbit_material(Material* material)=0;
    
    //! Returns the ground texture.  This is the material visible from
    //! the ground.
    //! @param material the material
    virtual void ground_material(Material* material)=0;
    
    //! Sets the number of rings used to generate the planet sphere.
    //! @param count the number of rings
    virtual void ring_count(size_t count)=0;
    
    //! Sets the number of divisions used to generate the planet sphere.
    //! This is equal to the number of lines of longitude.
    //! @param count the number of divisions
    virtual void division_count(size_t count)=0;
    
    //! Returns the number of detail levels.
    //! @param count the number of detail levels.
    virtual void detail_level_count(size_t count)=0;
};

}