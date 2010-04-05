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

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform sampler2D shadow_map;
uniform samplerCube environment_map;

struct LightResult {
    vec3 specular;
    vec3 diffuse;
    vec3 ambient;
};

LightResult phong() {
/*#ifdef NORMAL_MAP
    vec3 n = normalize(texture2D(normal_map, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
#else*/

    vec3 n = normalize(normal);
//#ifdef LIGHT_POINT
    vec3 light = vec3(gl_LightSource[0].position) - view;
/*#else
    vec3 light = vec3(gl_LightSource[0].position);
#endif

#endif*/

    vec3 v = normalize(view);
    vec3 r = reflect(v, n);
    vec3 l = normalize(light);
        
    // Calculate attenuation
    float d = length(light);
    float c0 = gl_LightSource[0].constantAttenuation;
    float c1 = gl_LightSource[0].linearAttenuation;
    float c2 = gl_LightSource[0].quadraticAttenuation;
    float a = 1.0 / (c0 + c1*d + c2*d*d);

    // Calculate diffuse and specular coefficients
    float s = a * max(0.0, dot(l, n));
    float t = a * max(0.0, pow(dot(l, r), gl_FrontMaterial.shininess));
    
    LightResult result;
    result.diffuse = vec3(s * gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse);
    if (s > 0.0) result.specular = vec3(t * gl_LightSource[0].specular * gl_FrontMaterial.specular);
    result.ambient = vec3(a * gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
    
    return result;
}

//#ifdef SHADOW_MAP
LightResult mul_shadow_map(in LightResult result) {
    vec4 shadow_coord = gl_TexCoord[1]/gl_TexCoord[1].w;
    float depth = texture2D(shadow_map, shadow_coord.st).z + 0.00002;
    if (depth < shadow_coord.z) {
        result.diffuse *= 0.2;
        result.specular *= 0.0;
    }
    return result;
}
//#endif


void main() {
    LightResult result = phong();
    
//#ifdef SHADOW_MAP
    result = mul_shadow_map(result);
//#endif
//#ifdef DIFFUSE_MAP
    result.diffuse *= texture2D(diffuse_map, gl_TexCoord[0].st);
/*#endif
#ifdef SPECULAR_MAP
    result.specular *= texture2D(specular_map, gl_TexCoord[0].st);
#endif*/
    
    gl_FragColor = vec4(result.ambient + result.diffuse + result.specular, 1.0);  
}