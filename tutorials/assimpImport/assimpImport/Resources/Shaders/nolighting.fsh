//
//  NoLighting.fsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//

precision highp float;

uniform sampler2D texture0;
varying vec2 v_texCoord;
varying vec4 v_vertex;

void main()
{
    gl_FragColor =  texture2D( texture0, v_texCoord);
}
