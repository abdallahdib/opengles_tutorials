precision highp float;

attribute vec4  position;
attribute vec3  normal;
attribute vec2  texCoord0;
attribute vec3  tangent;

uniform mat4  matProjViewModel;
uniform vec3  LightPosModel;          // Light position in model space

varying vec3  v_lightVec;
varying vec2  v_texCoord;

void main()
{
	gl_Position = matProjViewModel * position;
    //light direction
    vec3 lightDirection = normalize(LightPosModel /*- vec3(position)*/);// for point light: light direction is equal: light pos - vertex_pos
    //calculate bitangent
    vec3 bitangent = cross(normal, tangent);
    //create tangent stace 
    mat3 tangentSpaceXform = mat3(tangent, bitangent, normal);
    
    //transfrom light direction from model space to tangent space
	v_lightVec = lightDirection * tangentSpaceXform;
    
    //write tex coord for the current vertex so the pixel shader can interpolate tex coords on rasterized pixels
	v_texCoord = texCoord0;
}