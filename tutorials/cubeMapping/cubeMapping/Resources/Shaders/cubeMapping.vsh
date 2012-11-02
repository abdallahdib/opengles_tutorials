//per vertex attributes
attribute vec4 position;//vertex pos
attribute vec3 normal;// vertex normal

//shader uniforms
uniform vec4 camPosModel;//camera position in the model space model
uniform mat3 matModelToWorld;//rotation matrix of the model
uniform mat4 matProjViewModel;

//varying shader output variables 
varying vec3 v_reflectionVector;

void main()
{
	//get the eye direction vector in the model space
    vec4 eyeDir = normalize(position - camPosModel);
	
    //find the reflection vector between the eye direction and the vertex normal, multiply the reflection vector by the rotation matrix of the model ( transform to world space)
	v_reflectionVector = matModelToWorld * reflect(eyeDir.xyz, normal);
    
    //write final position
    gl_Position = matProjViewModel * position;

    
}