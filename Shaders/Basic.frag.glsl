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
 
varying vec3 normal;
varying vec3 view;

uniform sampler2D texture_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform samplerCube environment_map;
uniform sampler2D shadow_map;

vec3 phong(in vec3 n, in vec3 v) {
    vec3 specular = vec3(0);
    vec3 diffuse = vec3(0);
    vec3 ambient = vec3(0);
    
    vec4 shadow_coord = gl_TexCoord[1]/gl_TexCoord[1].w;
    float depth = texture2D(shadow_map, shadow_coord.st).z;
    float shadow = depth < shadow_coord.z ? 0.5 : 1.0;
    
    
    

    // Texture sampling
    /*vec4 tex_base = texture2D(tex, gl_TexCoord[0].st);
    vec4 spec_base;
    if (enable_spec_map) {
        spec_base = texture2D(spec, gl_TexCoord[0].st);
    } else {
        spec_base = vec4(1.0, 1.0, 1.0, 1.0);
    }*/
    vec3 r = reflect(v, n);
    
    // Two-sided Phong lighting shader.    
    for (int i = 0; i < gl_MaxLights; i++) {
        
        // Vector from the pixel to the light
        // Directional light
        vec3 light = vec3(gl_LightSource[i].position) - view;
        //vec3 light = vec3(gl_LightSource[i].position);
        vec3 l = normalize(light);
        
        // Calculate attenuation
        float d = length(light);
        float c0 = gl_LightSource[i].constantAttenuation;
        float c1 = gl_LightSource[i].linearAttenuation;
        float c2 = gl_LightSource[i].quadraticAttenuation;
        float a = 1.0 / (c0 + c1*d + c2*d*d);
    
        // Calculate diffuse and specular coefficients
        float s = a * max(0.0, dot(l, n));
        float t = a * max(0.0, pow(dot(l, r), gl_FrontMaterial.shininess));
        
        diffuse += s * gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * shadow;
        if (s > 0.0) {
            specular += t * gl_LightSource[i].specular * gl_FrontMaterial.specular * shadow;
        }
        ambient += a * gl_FrontMaterial.ambient * gl_LightSource[i].ambient;
        
    }
    
    return ambient + diffuse + specular;
}
 
void main() {
    vec3 n = normalize(normal);
    vec3 v = normalize(view);
    gl_FragColor = vec4(phong(n, v), 1.0);
    
    /*
    vec4 shadow = shadow_coord/shadow_coord.w;
    gl_FragColor = vec4(texture2D(shadow_map, shadow.st).z);*/
    
}