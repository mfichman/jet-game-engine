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
#include <Jet/Core/Material.hpp>
#include <Jet/Core/MeshObject.hpp>
#include <Jet/FractalPlanet.hpp>

namespace Jet { namespace Core {

//! Renders a planet that becomes more detailed as the camera approaches it.
//! @class FractalPlanet
//! Generates a planet.
class FractalPlanet : public Jet::FractalPlanet {
public:
    inline FractalPlanet(Engine* engine, Node* parent) :
        engine_(engine),
        parent_(parent),
        seed_(0),
        roughness_(1.0f),
        ring_count_(32),
        division_count_(32),
        detail_level_count_(5) {
            
        mesh_object_ = static_cast<MeshObject*>(parent_->mesh_object());
        mesh_ = static_cast<Mesh*>(engine_->mesh());
        mesh_object_->mesh(mesh_.get());
        mesh_object_->material("Rock.mtl");
        
        generate_mesh();
    }
    
    //! Returns the parent of this planet.
    inline Node* parent() const {
        return parent_;
    }
    
    //! Returns the seed used to generate the planet.
    inline size_t seed() const {
        return seed_;
    }
    
    //! Returns the roughness of the planet.  0 is the smoothest, and 1 is the
    //! roughest.
    inline float roughness() const {
        return roughness_;
    }
    
    //! Returns the orbit material.
    inline Material* orbit_material() const {
        return orbit_material_.get();
    }
    
    //! Returns the ground texture.
    inline Material* ground_material() const {
        return ground_material_.get();
    }
    
    //! Returns the number of rings used to generate the planet sphere.
    inline size_t ring_count() const {
        return ring_count_;
    }
    
    //! Returns the number of divisions used to generate the planet sphere.
    //! This is equal to the number of lines of longitude.
    inline size_t division_count() const {
        return division_count_;
    }
    
    //! Returns the number of detail levels.
    inline size_t detail_level_count() const {
        return detail_level_count_;
    }
    
    //! Sets the seed used to generate the planet.
    //! @param seed the random seed.
    inline void seed(size_t seed) {
        seed_ = seed;
    }

    //! Sets the roughness of the planet.  0 is the smoothest, and 1 is the
    //! roughest.
    //! @param roughness the roughness parameter
    inline void roughness(float roughness) {
        roughness_ = roughness;
    }
    
    //! Sets the orbit material.  This is the material visible from orbit.
    //! @param material the material
    inline void orbit_material(Jet::Material* material) {
        orbit_material_ = static_cast<Material*>(material);
    }
    
    //! Returns the ground texture.  This is the material visible from
    //! the ground.
    //! @param material the material
    inline void ground_material(Jet::Material* material) {
        ground_material_ = static_cast<Material*>(material);
    }
    
    //! Sets the number of rings used to generate the planet sphere.
    //! @param count the number of rings
    inline void ring_count(size_t count) {
        ring_count_ = count;
    }
    
    //! Sets the number of divisions used to generate the planet sphere.
    //! This is equal to the number of lines of longitude.
    //! @param count the number of divisions
    inline void division_count(size_t count) {
        division_count_ = count;
    }
    
    //! Returns the number of detail levels.
    //! @param count the number of detail levels.
    inline void detail_level_count(size_t count) {
        detail_level_count_ = count;
    }
    
private:
    //! Generates the mesh vertices at a level of detail appropriate for the
    //! distance to the camera.
    void generate_mesh();
    
    //! Generates a fractal quad using the given quad corners of the tile
    //! and the given detail level.
    void generate_quad(Vertex quad[4], size_t level);

	//! Generates the fractal noise for the quad.
	void generate_fractal(size_t offset, size_t level);

	//! Recalculates all normals.
	void recalculate_normals();
    
    Engine* engine_;
    Node* parent_;
    size_t seed_;
    float roughness_;
    MaterialPtr orbit_material_;
    MaterialPtr ground_material_;
    size_t ring_count_;
    size_t division_count_;
    size_t detail_level_count_;
    MeshObjectPtr mesh_object_;
    MeshPtr mesh_;
};

}}