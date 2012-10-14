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
    vec4 color =  texture2D( texture0, v_texCoord);
    vec2 fetch = v_texCoord.st * detailFactor;
    vec4 detail = texture2D(textureDetail, fetch);
    gl_FragColor =  color   * detail ;
}
