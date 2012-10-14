//
//  NoLighting.fsh
//  Virtual Vision
//
//  Created by Abdallah Dib on 4/10/11.
//  Copyright 2011 Virtual Vision. All rights reserved.
//



precision highp float;
uniform int enableDiscardScene;
uniform float waterHeight;
uniform sampler2D texture0;
uniform sampler2D textureDetail;
uniform int useDetailTexture;
uniform float detailFactor;

varying vec2 v_texCoord;
varying vec4 v_vertex;


void main()
{
    //we have to cull all objects under water for correct rendering of the mirrored texture
    if(enableDiscardScene == 1)
    {
        if(v_vertex.z < waterHeight)
            discard;// todo: this should be replaced with clip planes. its not recommanded to use discard operation.
    }
    
    vec4 color =  texture2D( texture0, v_texCoord);
    
    vec4 finalColor;
    vec2 fetch = v_texCoord.st * detailFactor;
    vec4 detail = texture2D(textureDetail, fetch);
    finalColor =  color   * detail;

    
    gl_FragColor = finalColor;

}
