precision highp float;

uniform sampler2D texture0;
uniform sampler2D textureBump;
		
varying vec3  v_lightVec;
varying vec2  v_texCoord;

void main()
{
	// per pixel normal 
    vec3 normal = texture2D(textureBump, v_texCoord).rgb * 2.0 - 1.0;
    //light intensity is the dot product of the light direction with the per pixel normal
    float lightIntensity = dot(v_lightVec, normal);

	//modulate light intensity with diffuse texture
    vec3 texColor = texture2D(texture0, v_texCoord).rgb;
	gl_FragColor = vec4(texColor * lightIntensity, 1.0);
}




