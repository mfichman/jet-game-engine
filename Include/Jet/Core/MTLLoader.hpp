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
#include <map>
#include <iostream>

namespace Jet { namespace Core {

//! This class loads a material using the Wavefront .MTL file format.
//! @class MatFactory
//! @brief Loads .OBJ files
class MTLLoader : public Loader {
public:
    //! Constructor.
    MTLLoader(Engine* engine);

    //! Destructor.
    virtual ~MTLLoader() {}

    //! Creates a new material from the given file.
    //! @param file the .MTL file
    virtual void resource(const std::string& file);

private:
    void newmtl(std::istream& in);
    void ambient(std::istream& in);
    void diffuse(std::istream& in);
    void specular(std::istream& in);
    void transparency(std::istream& in);
    void reflectivity(std::istream& in);
    void texture_map(std::istream& in);
    void specular_map(std::istream& in);
    void normal_map(std::istream& in);

    Engine* engine_;
    ComponentPtr material_;
    std::map<std::string, void (MTLLoader::*)(std::istream&)> command_;
};

}}
