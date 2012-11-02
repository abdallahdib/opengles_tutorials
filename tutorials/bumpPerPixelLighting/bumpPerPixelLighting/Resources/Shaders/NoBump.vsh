
precision highp float;

//per vertex attributes
attribute vec4  position;//vertex position
attribute vec3  normal;// vertex normal
attribute vec2  texCoord0;// vertex tex coordinate

//shader uniforms
uniform mat4  matProjViewModel;       // proj * view * model
uniform vec3  lightPosModel;          // Light position in model space
uniform vec3  camPosModel;          // camera position in model space


varying vec2 v_texCoord;
varying vec3 v_lightVector;
varying vec3 v_halfVector;
varying vec3 v_normal;

void main()
{
    // write final vertex position
    gl_Position =   matProjViewModel * position;
    
    // write tex coordinate for interpolation
    v_texCoord = texCoord0.xy;
    
    //write vertex normal for interpolation
    v_normal = normal ;
    
    //light direction in the model space
    v_lightVector = lightPosModel - position.xyz ;
    //normalize
    v_lightVector = normalize(v_lightVector);
    
    //half vector
    v_halfVector  = camPosModel - position.xyz ;
    v_halfVector = (v_halfVector + v_lightVector) /2.0;
    v_halfVector = normalize(v_halfVector);
}