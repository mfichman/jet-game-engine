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
varying vec3 light;

uniform mat4 shadow_matrix;

attribute vec3 binormal;

#undef NORMAL_MAP

void main() {
    // Transform to homogeneous coordinates
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
#ifdef SHADOW_MAP
    gl_TexCoord[1] = shadow_matrix * gl_Vertex;
#endif

#ifdef NORMAL_MAP
    vec3 view_position = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 b = normalize(gl_NormalMatrix * binormal);
    vec3 t = cross(n, b);
    
    // Tangent space calculation
    vec3 l = gl_LightSource[0].position.xyz - view_position;
    light.x = dot(l, t);
    light.y = dot(l, b);
    light.z = dot(l, n);
    view.x = dot(view_position, t);
    view.y = dot(view_position, b);
    view.z = dot(view_position, n);
#else

    normal = gl_NormalMatrix * gl_Normal;
    view = vec3(gl_ModelViewMatrix * gl_Vertex);
#endif
}
