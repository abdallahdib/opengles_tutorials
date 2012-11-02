//
//  SkyBox.vsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//

attribute vec4 position;
varying vec3 texCoord;
uniform mat4 matProjViewModel;

void main()
{
   gl_Position = matProjViewModel * ( position );
   texCoord.xyz = position.xyz;
}



