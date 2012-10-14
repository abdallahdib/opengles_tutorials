//
//  NoLighting.fsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//


precision highp float;

uniform sampler2D texture0;
uniform sampler2D textureDetail;
uniform float detailFactor;

varying vec2 v_texCoord;



void main()
{    
    vec4 finalColor;
    vec4 color =  texture2D( texture0, v_texCoord);
    vec2 fetch = v_texCoord.st * detailFactor;
    vec4 detail = texture2D(textureDetail, fetch);
    finalColor =  color   * detail ;

      //hard coded per pixel fog, its very expensive in a real time application to to perform exponential fog per pixel,
     //instead, calculate fog factor per vertex and write result to a varying variable .
     float z = gl_FragCoord.z / gl_FragCoord.w;
     vec4 fogColor = vec4(0.8, 0.8, 0.8, 1.0);
     float fogFactor = exp( -0.003 * z );
     fogFactor = clamp(fogFactor, 0.0, 1.0);
     
     //write final color
     gl_FragColor = mix(fogColor,finalColor,fogFactor);

}
