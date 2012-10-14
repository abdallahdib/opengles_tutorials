//
//  GpuSkinnig.vsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//


varying vec2 v_texCoord;

attribute vec4 position;
attribute vec2 texCoord0;
//influenced bones with their corresponding weight
attribute vec4 bones;
attribute vec4 weights;

uniform mat4 matProjViewModel;
//bones transformation at a given time
#define JOINT_COUNT 60
uniform mat4 skinningMatrix[JOINT_COUNT];

void main()
{
	v_texCoord = texCoord0;
    
    //each vertex position is affected by certain number of bones,
    //vertex attributes holds the index of influenced bone and the corresponding weight.
    //for eg: a vertex 'vert' is influenced by 'bone0' with 'weight0' and 'bone1' with 'weight1'.
    //the final position of 'vert' is:
    //vert.finalpos = weight0 * (bone0.matrix * vert.pos) + weight1 * (bone1.matrix * vert.pos);
    
    //here we suppose that a vertex is affected by four bones, for optimization we can reduce this to two if we have animated models with maximum bones per vertex equal to two.
    vec4 p0 = skinningMatrix[ int(bones.x) ] * position;
    vec4 p1 = skinningMatrix[ int(bones.y) ] * position;
    vec4 p2 = skinningMatrix[ int(bones.z) ] * position;
    vec4 p3 = skinningMatrix[ int(bones.w) ] * position;
    
    //blend position  
    vec4 interpolatedPosition = p0 * weights.x + p1 * weights.y + p2 * weights.z + p3 * weights.w;
    
    //write final position
    gl_Position = matProjViewModel * interpolatedPosition;

}
