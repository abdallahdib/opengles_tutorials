//
//  NoLighting.vsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//

attribute vec4 position;
attribute vec2 texCoord0;

varying vec2 v_texCoord; 

uniform mat4 matModel;
uniform mat4 matProjViewModel;

varying vec4 v_vertex;
void main()
{
	v_texCoord = texCoord0; 
	gl_Position = matProjViewModel * position;
    v_vertex =  matModel * position;
}
