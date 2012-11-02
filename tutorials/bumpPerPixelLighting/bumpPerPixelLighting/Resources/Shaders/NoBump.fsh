precision highp float;

//diffuse texture
uniform sampler2D texture0;

//lighting properties
uniform vec4 lightColorAmbient;//light ambient color
uniform vec4 lightColorDiffuse;//light diffuse color
uniform vec4 lightColorSpecular;//light specular color

//material properties
uniform vec4 matColorAmbient;//mat ambient
uniform vec4 matColorDiffuse;//material diffuse
uniform vec4 matColorSpecular;//material specular
uniform float matShininess;//material shininess

//repeat tex factor
uniform float repeatTexture;

//varying input
varying vec2 v_texCoord;
varying vec3 v_lightVector;
varying vec3 v_halfVector;
varying vec3 v_normal;

void main()
{
    //tex coordinate * repeat factor
    vec2 texCoord = v_texCoord * repeatTexture;
    
    //fetch per vertex diffuse color
	vec4 color  = texture2D(texture0, texCoord);

    
    //invert normal for back faces ( if face culling is enabled this can be removed)
    vec3 normal = v_normal;
    if (!gl_FrontFacing)
        normal = -normal;
    
    //calculate light contribution
    //1- diffuse or lamber factor
    float lamber  =  max(0.0, dot(v_lightVector, normal) );
    
    //2- specular factor
    float specular = 0.0;
    if (dot(normal, v_lightVector) < 0.0)
        specular = 0.0;
    else
        specular = max(0.0, pow(dot(normalize(v_halfVector), normal), matShininess)) ;
    
    //get the final ambient diffuse and specular color
	vec4 finalAmbientContrib  = lightColorAmbient  * color /** matColorAmbient.xyz*/;
	vec4 finalDiffuseContrib  = lightColorDiffuse  * color *  lamber * matColorDiffuse;
	vec4 finalSpecularContrib = lightColorSpecular * specular * matColorSpecular;
    
    //the final color is the sum of ambient dffuse and specular
    gl_FragColor = vec4( finalAmbientContrib + (finalDiffuseContrib + finalSpecularContrib))  ;
}




