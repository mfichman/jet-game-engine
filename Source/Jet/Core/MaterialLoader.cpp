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

#include <Jet/Core/MaterialLoader.hpp>
#include <Jet/Engine.hpp>
#include <fstream>

using namespace Jet::Core;
using namespace Jet;
using namespace std;
using namespace boost;

MaterialLoader::MaterialLoader(Engine* engine) :
    engine_(engine) {

	command_["newmtl"] = &MaterialLoader::newmtl;
    command_["Ka"] = &MaterialLoader::ambient;
    command_["Kd"] = &MaterialLoader::diffuse;
    command_["Ks"] = &MaterialLoader::specular;
    command_["d"] = &MaterialLoader::transparency;
    command_["tr"] = &MaterialLoader::transparency;
    command_["r"] = &MaterialLoader::reflectivity;
    command_["map_Ka"] = &MaterialLoader::texture_map;
    command_["map_Kd"] = &MaterialLoader::texture_map;
    command_["map_Ks"] = &MaterialLoader::specular_map;
    command_["map_bump"] = &MaterialLoader::normal_map;
}

void MaterialLoader::newmtl(istream& in) {
    string value;
    in >> value;
    material_ = engine_->material(value);
}

void MaterialLoader::ambient(istream& in) {
    real_t red, blue, green;
    in >> red >> blue >> green;
    material_->ambient_color(Color(red, blue, green, 1.0f));  
}

void MaterialLoader::diffuse(istream& in) {
    real_t red, blue, green;
    in >> red >> blue >> green;
    material_->diffuse_color(Color(red, blue, green, 1.0f)); 
}

void MaterialLoader::specular(istream& in) {
    real_t red, blue, green;
    in >> red >> blue >> green;
    material_->specular_color(Color(red, blue, green, 1.0f)); 
}

void MaterialLoader::transparency(istream& in) {
    real_t value;
    in >> value;
	//throw runtime_error("Transparency not implemented");
}

void MaterialLoader::reflectivity(istream& in) {
    real_t value;
    in >> value;
	//throw runtime_error("Reflectivity not implemented");
}

void MaterialLoader::texture_map(istream& in) {
    string value;
    in >> value;
    material_->diffuse_map(value);
}

void MaterialLoader::specular_map(istream& in) {
    string value;
    in >> value;
    material_->specular_map(value);
}

void MaterialLoader::normal_map(istream& in) {
    string value;
    in >> value;
    material_->normal_map(value);
}
  
void MaterialLoader::resource(const std::string& name) {
    static const std::string& ext = ".mtl";
    if ((name.length() - name.rfind(ext)) != ext.length()) {
        throw runtime_error("Invalid file extension");
    }
	
	std::string file = engine_->resource_path(name);
    ifstream in(file.c_str());
    string command;
    
    while (in.good()) {
        in >> command;

		if (!in.good()) break;
        
        if (command.find("#") == 0) {
            // Skip the comment line
            string line;
            getline(in, line);
        } else {
            map<string, void (MaterialLoader::*)(istream&)>::iterator i = command_.find(command);
            if (i != command_.end()) {
                ((this)->*(i->second))(in);
            }
        }
    }  
}
