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

#define MAX_CASCADES 4
#define NORMAL_MAP
#define SHADOW_MAP
#define SPECULAR_MAP
#define DIFFUSE_MAP

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform sampler2DShadow shadow_map[MAX_CASCADES];
uniform samplerCube environment_map;
uniform int light_count;
uniform int cascade_count;
uniform float shadow_z[MAX_CASCADES];
uniform float shadow_distance;

uniform bool diffuse_map_enabled;
uniform bool specular_map_enabled;
uniform bool normal_map_enabled;
uniform bool shadow_map_enabled;

varying vec3 eye_dir;
varying vec3 light_dir;
varying vec4 shadow_coord[MAX_CASCADES];

float shadow_lookup(sampler2DShadow shadow_sampler, vec4 shadow_coord) {
    shadow_coord.z -= 0.0005;
    return shadow2DProj(shadow_sampler, shadow_coord).w;
}

float shadow_pcf_lookup(sampler2DShadow shadow_sampler, vec4 shadow_coord, vec2 offset) {
    float x_offset = 1.0/2048.0;
    float y_offset = 1.0/2048.0;
    shadow_coord.x += offset.x * x_offset * shadow_coord.w;
    shadow_coord.y += offset.y * y_offset * shadow_coord.w;
    shadow_coord.z -= 0.00005;
    return shadow2DProj(shadow_sampler, shadow_coord).w;
}

float shadow_pcf(sampler2DShadow shadow_sampler, vec4 shadow_coord) {
    vec2 o = mod(floor(vec2(gl_FragCoord.xy)), 2.0);
    float shadow = 0.0;
    shadow += shadow_pcf_lookup(shadow_sampler, shadow_coord, vec2(-1.5, 1.5) + o);
    shadow += shadow_pcf_lookup(shadow_sampler, shadow_coord, vec2(0.5, 1.5) + o);
    shadow += shadow_pcf_lookup(shadow_sampler, shadow_coord, vec2(-1.5, -0.5) + o);
    shadow += shadow_pcf_lookup(shadow_sampler, shadow_coord, vec2(0.5, -0.5) + o);                                
    shadow /= 4.0;
    
    return shadow;
}


void phong_color(inout vec4 diffuse, inout vec4 specular, inout vec4 ambient) {
    // Calculate the tangent, binormal, and normal vectors
    vec3 n = vec3(0.0, 0.0, 1.0);
#ifdef NORMAL_MAP
    if (normal_map_enabled) {
        n = normalize(texture2D(normal_map, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
    }
#endif
    for (int i = 0; i < 1; i++) {
        // Calculate light vector
        vec3 v = normalize(eye_dir);
        vec3 l = normalize(light_dir);
        vec3 r = reflect(v, n);
    
        // Calculate diffuse and specular coefficients
        float kd = max(0.0, dot(l, n));
        float ks = pow(max(0.0, dot(l, r)), gl_FrontMaterial.shininess);
            
        // Calculate ambient, diffuse, and specular light
        diffuse += kd * gl_LightSource[i].diffuse;
        specular += ks * gl_LightSource[i].specular;
        ambient += gl_LightSource[i].ambient;
    }
}

void material_color(inout vec4 diffuse, inout vec4 specular, inout vec4 ambient) {
    diffuse *= gl_FrontMaterial.diffuse;
    specular *= gl_FrontMaterial.specular;
    ambient *= gl_FrontMaterial.ambient;
}

void shadow_color(inout vec4 diffuse, inout vec4 specular, inout vec4 ambient) {
#ifdef SHADOW_MAP
    float z = gl_FragCoord.z/gl_FragCoord.w;
    if (shadow_map_enabled && z < shadow_distance) {
        float shadow = 0.0;
        if (z < shadow_z[0]) {
            shadow += shadow_lookup(shadow_map[0], shadow_coord[0]);
        } else if (z < shadow_z[1]) {
            shadow += shadow_lookup(shadow_map[1], shadow_coord[1]);
        } else if (z < shadow_z[2]) {
            shadow += shadow_lookup(shadow_map[2], shadow_coord[2]);
        } else {
            shadow += shadow_lookup(shadow_map[3], shadow_coord[3]);
            float fade = (shadow_z[3] - z)/(shadow_z[3] - shadow_z[2]);
            shadow = mix(1.0, shadow, sqrt(fade));
        }
        
        diffuse *= 0.5 + 0.5 * shadow;
        specular *= 0.1 + 0.9 * shadow;
    }
#endif
}

void diffuse_color(inout vec4 diffuse, inout vec4 specular, inout vec4 ambient) {
#ifdef DIFFUSE_MAP
    if (diffuse_map_enabled) {
        vec4 color = texture2D(diffuse_map, gl_TexCoord[0].st);
        diffuse *= color;
        ambient *= color;
    }
#endif
}

void specular_color(inout vec4 diffuse, inout vec4 specular, inout vec4 ambient) {
#ifdef SPECULAR_MAP
    if (specular_map_enabled) {
        specular *= texture2D(specular_map, gl_TexCoord[0].st);
    }
#endif
}

void fog_color() {
    
    float z = gl_FragCoord.z/gl_FragCoord.w;
    float density = 0.0065;
    float fog = clamp(exp2(-density * density * z * z), 0.0, 1.0);   
    gl_FragColor = mix(vec4(0.0, 0.0, 0.0, 0.0), gl_FragColor, fog);
}

void main() {
    vec4 diffuse = vec4(0);
    vec4 specular = vec4(0);
    vec4 ambient = vec4(0);
    phong_color(diffuse, specular, ambient);
    material_color(diffuse, specular, ambient);
    diffuse_color(diffuse, specular, ambient);
    specular_color(diffuse, specular, ambient);
    shadow_color(diffuse, specular, ambient);

    gl_FragColor = vec4(ambient + diffuse + specular);
    
    // fog_color()
    
}
