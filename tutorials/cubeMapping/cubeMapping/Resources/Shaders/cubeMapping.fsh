uniform samplerCube textureCubeMap;
varying mediump vec3  v_reflectionVector;

void main()
{
    gl_FragColor = textureCube(textureCubeMap, v_reflectionVector);
}