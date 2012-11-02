
precision highp float;

//per vertex attribute
attribute vec4  position;   //vertex pos
attribute vec3  normal;     //vertex normal
attribute vec2  texCoord0;  //vertex tex coord
attribute vec3  tangent;    // vertex tangent

//shader uniform
uniform mat4  matProjViewModel;      //Proj * View * Model
uniform vec3  lightPosModel;        //light position in the model space
uniform vec3  camPosModel;         //camera position in the model space

//shader varying output
varying vec2  v_texCoord;
varying vec3 v_lightVector;
varying vec3 v_halfVector;

void main()
{
    // write final vertex position
    gl_Position =   matProjViewModel * position;
    
    // write tex coordinate for interpolation
    v_texCoord = texCoord0.xy;
    
    //calculate bitangent ( this can be don on cpu)
    vec3 bitangent = cross(normal, tangent);
    //create tangent space
    mat3 tangentSpace = mat3(tangent, bitangent, normal);
    
    //get light direction for the current vertex
    v_lightVector =  lightPosModel - position.xyz  ;
    //transform light direction to tangent space
    v_lightVector = v_lightVector * tangentSpace;
    //normalize
    v_lightVector = normalize(v_lightVector);

    //get eye direction for the current vertex
    v_halfVector = camPosModel - position.xyz ;
    //transfrom to tangent space
    v_halfVector = v_halfVector * tangentSpace;
    //normalize
    v_halfVector = normalize(v_halfVector);
    //calculate the half vector
    v_halfVector = (v_halfVector + v_lightVector) /2.0;
    //normalize
    v_halfVector = normalize(v_halfVector) ;
}