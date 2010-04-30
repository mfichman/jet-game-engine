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
#include <Jet/Core/Mesh.hpp>
#include <vector>
#include <map>
#include <fstream>

namespace Jet { namespace Core {

//! Loads mesh data from an OBJ file.
//! @class Mesh
//! @brief Loads mesh data from an OBJ file.
class MeshLoader : public Object {
public:
    //! Creates a new mesh loader for the given mesh, and loads data into
    //! that mesh.
    //! @param mesh the mesh to loader
    //! @param file path to the file to be loaded
    MeshLoader(Mesh* mesh, const std::string& path);
    
private:
	void read_face();
	void insert_face(Vertex face[3]);
	void compute_tangent(Vertex face[3], size_t j);
    
    MeshPtr mesh_;
	std::map<Vertex, uint32_t> cache_;
    std::ifstream in_;
    std::string name_;
    std::vector<Vector> position_;
	std::vector<Vector> normal_;
	std::vector<Texcoord> texcoord_;
};

}}