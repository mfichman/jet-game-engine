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

#include <Jet/Core/MTLLoader.hpp>
#include <Jet/Engine.hpp>
#include <fstream>

using namespace Jet::Core;
using namespace Jet;
using namespace std;
using namespace boost;

MTLLoader::MTLLoader(Engine* engine) :
    engine_(engine) {

	command_["newmtl"] = &MTLLoader::newmtl;
    command_["Ka"] = &MTLLoader::ambient;
    command_["Kd"] = &MTLLoader::diffuse;
    command_["Ks"] = &MTLLoader::specular;
    command_["d"] = &MTLLoader::transparency;
    command_["tr"] = &MTLLoader::transparency;
    command_["r"] = &MTLLoader::reflectivity;
    command_["map_Ka"] = &MTLLoader::texture_map;
    command_["map_Kd"] = &MTLLoader::texture_map;
    command_["map_Ks"] = &MTLLoader::specular_map;
    command_["map_bump"] = &MTLLoader::normal_map;
}

void MTLLoader::newmtl(istream& in) {
    string value;
    in >> value;
    material_ = engine_->component(value);
    material_->type("Material");
}

void MTLLoader::ambient(istream& in) {
    real_t red, blue, green;
    in >> red >> blue >> green;
    material_->value("ambient", Color(red, blue, green, 1.0f));  
}

void MTLLoader::diffuse(istream& in) {
    real_t red, blue, green;
    in >> red >> blue >> green;
    material_->value("diffuse", Color(red, blue, green, 1.0f)); 
}

void MTLLoader::specular(istream& in) {
    real_t red, blue, green;
    in >> red >> blue >> green;
    material_->value("specular", Color(red, blue, green, 1.0f)); 
}

void MTLLoader::transparency(istream& in) {
    real_t value;
    in >> value;
    material_->value("transparency", value);
}

void MTLLoader::reflectivity(istream& in) {
    real_t value;
    in >> value;
    material_->value("reflectivity", value);

}

void MTLLoader::texture_map(istream& in) {
    string value;
    in >> value;
    material_->value("diffuse_map", value);
}

void MTLLoader::specular_map(istream& in) {
    string value;
    in >> value;
    material_->value("specular_map", value);
}

void MTLLoader::normal_map(istream& in) {
    string value;
    in >> value;
    material_->value("normal_map", value);
}
  
void MTLLoader::resource(const std::string& file) {
    static const std::string& ext = ".mtl";
    if ((file.length() - file.rfind(ext)) != ext.length()) {
        throw runtime_error("Invalid file extension");
    }

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
            map<string, void (MTLLoader::*)(istream&)>::iterator i = command_.find(command);
            if (i != command_.end()) {
                ((this)->*(i->second))(in);
            }
        }
    }  
}
