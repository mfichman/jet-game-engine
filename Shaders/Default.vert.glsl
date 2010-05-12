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

#define SHADOW_MAP

varying vec3 eye_dir;
varying vec3 light_dir;

varying vec4 shadow_coord0;
varying vec4 shadow_coord1;
varying vec4 shadow_coord2;
varying vec4 shadow_coord3;

attribute vec3 tangent;

void main() {
    
    eye_dir = vec3(gl_ModelViewMatrix * gl_Vertex);
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 t = normalize(gl_NormalMatrix * tangent);
    vec3 b = cross(n, t);
    
    vec3 v;
    v.x = dot(gl_LightSource[0].position.xyz, t);
    v.y = dot(gl_LightSource[0].position.xyz, b);
    v.z = dot(gl_LightSource[0].position.xyz, n);
    light_dir = v;
    
    v.x = dot(eye_dir, t);
    v.y = dot(eye_dir, b);
    v.z = dot(eye_dir, n);
    eye_dir = v;
    
#ifdef SHADOW_MAP
    shadow_coord0 = gl_TextureMatrix[3] * gl_TextureMatrix[0] * gl_Vertex;
    shadow_coord1 = gl_TextureMatrix[4] * gl_TextureMatrix[0] * gl_Vertex;
    shadow_coord2 = gl_TextureMatrix[5] * gl_TextureMatrix[0] * gl_Vertex;
    shadow_coord3 = gl_TextureMatrix[6] * gl_TextureMatrix[0] * gl_Vertex;
#endif
}
