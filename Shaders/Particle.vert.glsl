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

uniform float time;
uniform float scale;

attribute vec3 init_position;
attribute vec3 init_velocity;
attribute float init_time;
attribute float init_size;
attribute float init_rotation;
attribute float life;
attribute float growth_rate;

varying float alpha;
varying float rotation;

void main() {
    float elapsed_time = time - init_time;
    vec3 world_position = init_position + elapsed_time * init_velocity;
    vec4 view_position = gl_ModelViewMatrix * vec4(world_position, 1.0);
    float distance = length(view_position.xyz);
    
    float x = elapsed_time/life;
    float f = (-exp((x+0.03)/1.5)-exp(-65.0*(x+0.03)/1.5)+0.2*exp(2.2*(x+0.03)/1.5)+1.08)*4.5;
    //float f = 18.0 * sin(x) * exp(-6.0*x);
    
    gl_Position = gl_ProjectionMatrix * view_position;
    gl_PointSize = max(scale * init_size / (1.0 + distance) + growth_rate * x, 0.0);
   // gl_TexCoord[0] = gl_MultiTexCoord0;
    alpha = clamp(f, 0.0, 1.0);
    rotation = init_rotation;
}
