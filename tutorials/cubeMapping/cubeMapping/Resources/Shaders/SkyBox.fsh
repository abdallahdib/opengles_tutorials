//
//  SkyBox.fsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//

precision mediump float;
uniform samplerCube textureCubeMap;
varying vec3 texCoord;

void main()
{
    gl_FragColor = vec4(textureCube(textureCubeMap, texCoord.xyz));
}




