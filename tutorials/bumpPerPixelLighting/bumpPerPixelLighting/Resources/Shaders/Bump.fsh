
precision highp float;

uniform sampler2D texture0;//diffuse texture
uniform sampler2D textureBump;//bump texture
//uniform sampler2D textureSpecular;//if the model has a specular texture => uncomment
        
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
varying vec2 v_texCoord;// per pixel tex coordinate
varying vec3 v_lightVector;// per pixel light dir vector
varying vec3 v_halfVector;// per pixel half vector

void main()
{
    //tex coordinate * repeat factor
    vec2 texCoord = v_texCoord * repeatTexture; 

    //fetch per vertex diffuse color
	vec4 color  = texture2D(texture0, texCoord) ;
    
	//fetch per pixel normal
    vec3 bump = texture2D(textureBump, texCoord).rgb * 2.0 - 1.0;
    //invert normal for back faces ( if face culling is enabled this can be removed)
    if (!gl_FrontFacing)
        bump = - bump;
    
    //calculate light contribution
    //1- lamber or diffuse factor
    float lamber = max(0.0, dot(normalize(v_lightVector), bump) );
    
    //2- specular factor
    float specular = 0.0;
    if (dot(bump, v_lightVector) < 0.0)
        specular = 0.0;
    else
        specular = max(0.0, pow(dot(normalize(v_halfVector), bump), matShininess)) ;

    //get the final ambient diffuse and specular color
	vec4 finalAmbientContrib  = lightColorAmbient  * color /** matColorAmbient.xyz*/;
	vec4 finalDiffuseContrib  = lightColorDiffuse  * color *  lamber * matColorDiffuse;
	vec4 finalSpecularContrib = lightColorSpecular  * specular * matColorSpecular;
    
    //the final color is the sum of ambient dffuse and specular
    gl_FragColor = finalAmbientContrib + (finalDiffuseContrib + finalSpecularContrib)  ;

}




