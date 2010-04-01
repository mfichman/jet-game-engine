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

#include <Jet/Loader.hpp>
#include <iostream>
#include <vector>
#include <map>

namespace Jet { namespace Core {

//! This class loads a mesh using the Wavefront .OBJ file format.
//! @class OBJLoader
//! @brief Loads .OBJ files
class OBJLoader : public Loader {
public:
    //! Constructor
    OBJLoader(Engine* engine);

    //! Destructor
    virtual ~OBJLoader() {}

    //! Creates a new mesh from the given file.
    //! @param file the .OBJ file
    virtual void resource(const std::string& file);

private:
    void face(std::istream& in);
    void vertex(std::istream& in);
    void normal(std::istream& in);
    void texcoord(std::istream& in);
    void mtllib(std::istream& in);
    void usemtl(std::istream& in);
    void binormal(Vertex face[3], size_t j);

    Engine* engine_;
    std::vector<Vector> position_;
    std::vector<Vector> normal_;
    std::vector<Texcoord> texcoord_;
    std::map<Vertex, uint32_t> vertex_;
    std::vector<uint32_t> index_;
    uint32_t cur_index_;
    std::string material_;
    std::map<std::string, void (OBJLoader::*)(std::istream&)> command_;

};

}}
