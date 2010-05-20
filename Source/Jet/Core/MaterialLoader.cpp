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
#include <fstream>
#include <sstream>
 
using namespace Jet;
using namespace std;

Core::MaterialLoader::MaterialLoader(Material* material, const std::string& file) {
    ifstream in(file.c_str());
    string command;
    
    while (in.good()) {
        in >> command;

		if (in.fail()) break;
        
        if (command.find("#") == 0) {
            // Skip the comment line
            string line;
            getline(in, line);
        } else if (command == "Ka") {
			float red, blue, green;
			in >> red >> blue >> green;
			material->ambient_color(Color(red, blue, green, 1.0f));  
		} else if (command == "Kd") {
			float red, blue, green;
			in >> red >> blue >> green;
			material->diffuse_color(Color(red, blue, green, 1.0f)); 
		} else if (command == "Ks") {
			float red, blue, green;
			in >> red >> blue >> green;
			material->specular_color(Color(red, blue, green, 1.0f)); 
		} else if (command == "map_Kd") {
			string value;
			in >> value;
			material->diffuse_map(value);
		} else if (command == "map_Ks") {
			string value;
			in >> value;
			material->specular_map(value);
		} else if (command == "map_bump") {
			string value;
			in >> value;
			material->normal_map(value);
		} else if (command == "Ns") {
			float value;
			in >> value;
			material->shininess(value);
		} else if (command == "double_sided") {
            material->double_sided(true);
		} else {
			// Unknown command, skip the rest of the line
			string line;
            getline(in, line);
		}
    }
}