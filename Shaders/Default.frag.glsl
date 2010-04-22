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
varying vec3 tangent;
varying vec3 view;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform sampler2D shadow_map;
uniform samplerCube environment_map;
uniform int light_count;

uniform bool diffuse_map_enabled;
uniform bool specular_map_enabled;
uniform bool normal_map_enabled;
uniform bool shadow_map_enabled;

//#define NORMAL_MAP
#define SHADOW_MAP
//#define SPECULAR_MAP
#define DIFFUSE_MAP

void main() {
    
    // Calculate the tangent, binormal, and normal vectors
    vec3 n = normalize(normal);
#ifdef NORMAL_MAP
    if (normal_map_enabled) {
        vec3 t = normalize(tangent);
        vec3 b = normalize(cross(t, n));
        mat3 tbn_matrix = mat3(t, b, n);
        vec3 n_sample = normalize(texture2D(normal_map, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
        n = tbn_matrix * n_sample;
    }
#endif

    vec4 diffuse, specular, ambient;
    
    for (int i = 0; i < 1; i++) {
        // Calculate light vector
        vec3 light = vec3(gl_LightSource[i].position) - view*gl_LightSource[i].position.w;
    
        // Calculate attenuation
        float d = length(light);
        float c0 = gl_LightSource[i].constantAttenuation;
        float c1 = gl_LightSource[i].linearAttenuation;
        float c2 = gl_LightSource[i].quadraticAttenuation;
        float a = 1.0 / (c0 + c1*d + c2*d*d);
        
        // Calculate view, light, and reflection vectors
        vec3 v = normalize(view);
        vec3 l = normalize(-light);
        vec3 r = reflect(v, n);
    
        // Calculate diffuse and specular coefficients
        float kd = a * max(0.0, dot(l, n));
        float ks = a * max(0.0, pow(dot(l, r), gl_FrontMaterial.shininess));
            
        // Calculate ambient, diffuse, and specular light
        diffuse += kd * gl_LightSource[i].diffuse;
        specular += ks * gl_LightSource[i].specular;
        ambient += a * gl_LightSource[i].ambient;
    }
    
    diffuse *= gl_FrontMaterial.diffuse;
    specular *= gl_FrontMaterial.specular;
    ambient *= gl_FrontMaterial.ambient;
    
#ifdef DIFFUSE_MAP
    if (diffuse_map_enabled) {
        diffuse *= texture2D(diffuse_map, gl_TexCoord[0].st);
    }
#endif
#ifdef SPECULAR_MAP
    if (specular_map_enabled) {
        specular *= texture2D(specular_map, gl_TexCoord[0].st);
    }
#endif
#ifdef SHADOW_MAP
    if (shadow_map_enabled) {
        vec4 shadow_coord = gl_TexCoord[1]/gl_TexCoord[1].w;
        float depth = texture2D(shadow_map, shadow_coord.st).z + 0.001;
        if (depth < shadow_coord.z) {
            diffuse *= 0.4;
            specular *= 0.2;
        }
    }
#endif
    
    vec4 color = ambient + diffuse + specular;
    float intensity = color.r + color.g + color.b;
    gl_FragData[0] = color;
    
    // High-pass filter
    if (intensity > 3.0) {
        gl_FragData[1] = color;
    }
    
}