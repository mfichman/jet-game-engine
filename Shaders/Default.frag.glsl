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

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform sampler2DShadow shadow_map;
uniform samplerCube environment_map;
uniform int light_count;
uniform float shadow_distance;

uniform bool diffuse_map_enabled;
uniform bool specular_map_enabled;
uniform bool normal_map_enabled;
uniform bool shadow_map_enabled;


#define NORMAL_MAP
#define SHADOW_MAP
#define SPECULAR_MAP
#define DIFFUSE_MAP

varying vec3 eye_dir;
varying vec3 light_dir;


float shadow_lookup(vec2 offset)
{
    float x_offset = 1.0/2048.0;
    float y_offset = 1.0/2048.0;
    vec4 shadow_coord = gl_TexCoord[1];
    shadow_coord.x += offset.x * x_offset * gl_TexCoord[1].w;
    shadow_coord.y += offset.y * y_offset * gl_TexCoord[1].w;
    shadow_coord.z -= 0.0005;
    return shadow2DProj(shadow_map, shadow_coord).w;

}

void main() {
    
    // Calculate the tangent, binormal, and normal vectors
    vec3 n = vec3(0.0, 0.0, 1.0);
#ifdef NORMAL_MAP
    if (normal_map_enabled) {
        n = normalize(texture2D(normal_map, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
    }
#endif

    vec4 diffuse, specular, ambient;
    
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
    float z = gl_FragCoord.z/gl_FragCoord.w;
    if (shadow_map_enabled && z < shadow_distance) {
        
        float shadow = 0.0;             
        vec2 o = mod(floor(gl_FragCoord.xy), 2.0);
        shadow += shadow_lookup(vec2(-1.5, 1.5) + o);
        shadow += shadow_lookup(vec2( 0.5, 1.5) + o);
        shadow += shadow_lookup(vec2(-1.5, -0.5) + o);
        shadow += shadow_lookup(vec2( 0.5, -0.5) + o);                                
        shadow /= 4.0;
        
        //shadow = shadow_lookup(vec2(0, 0));
        /*
        float x, y;
        for (y = -1.5; y <= 1.5; y += 1.0) {
            for (x = -1.5; x <= 1.5; x += 1.0) {
                shadow += shadow_lookup(vec2(x, y));
            }
        }
        shadow /= 16.0;
        */
        
        float ratio = pow(clamp(z/shadow_distance, 0.0, 1.0), 2.0);
        shadow = (1.0 - ratio) * shadow + ratio;        
        
        diffuse *= 0.4 + 0.6 * shadow;
        specular *= 0.1 + 0.9 * shadow;
               

        
    }
#endif


    
    
    gl_FragColor = vec4(ambient + diffuse + specular);
    
    //float z = gl_FragCoord.z/gl_FragCoord.w;
    //float density = 0.0065;
    //float fog = clamp(exp2(-density * density * z * z), 0.0, 1.0);   
    //gl_FragColor = mix(vec4(0.0, 0.0, 0.0, 0.0), gl_FragColor, fog);
}
