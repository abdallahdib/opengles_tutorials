//
//  WaterSurface.fsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//


precision mediump float;

uniform sampler2D texture0;
uniform sampler2D normal;
uniform float time;
uniform float screenWidth;
uniform float screenHeight;

varying vec2 v_texCoord;

void main()
{
    //this define the number to water surface tile
    const float tile_factor = 20.0;
    const float noise_factor = 0.03;
    
    vec2 texCoordNormal0 = v_texCoord * tile_factor;
    texCoordNormal0 += time ;

    vec2 texCoordNormal1 = v_texCoord * tile_factor;
    texCoordNormal1.s -= time ;
    texCoordNormal1.t += time ;

    vec3 normal0 = texture2D(normal, texCoordNormal0).rgb * 2.0 - 1.0;
    vec3 normal1 = texture2D(normal, texCoordNormal1).rgb * 2.0 - 1.0;
    vec3 normal = normalize(normal0 + normal1);
    
    //adjust texture coordinate based on screen size
    vec2 texCoordReflection = vec2(gl_FragCoord.x * screenWidth, gl_FragCoord.y * screenHeight);
    
    //the final texture cooridnate is disturbed using the normal texture with some noise factor 
    vec2 texCoordFinal = texCoordReflection.xy + noise_factor * normal.xy;

    gl_FragColor =  texture2D( texture0, texCoordFinal);
}




