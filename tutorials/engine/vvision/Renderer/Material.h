/* Material.h
 *
 * Virtual Vision Engine . Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/


#ifndef MATERIAL_INCLUDED
#define MATERIAL_INCLUDED

#include "vmath.h"
#include "Texture.h"
#include "TextureCubeMap.h"

namespace vvision
{
    /** this is the material struct, each group in the model has materials
     a mesh is represented by a number of MeshGroups and materials*/
    class CMaterial
    {
        
        /** public attributes*/
    public:
        /** the material name*/
        std::string Name;
        
        /** ambient color*/
        vec4f ambient;
        
        /** diffuse color*/
        vec4f diffuse;
        
        /** specular*/
        vec4f specular;
        
        /** emissive*/
        vec4f emissive;
        
        /** shininess*/
        float32 shininess;
        
        /**material opacity */
        float32 opacity;//0: full transparency 1: opaque
        
        /** Specifies whether meshes using this material must be rendered without backface culling. 0 for false, !0 for true.*/
        int32 twoSided; // if = 1 render without faceculling if =0 enable face culling default: 0 (face culling is enabled)
        
        /** this tell if the material is transparent*/
        bool isTransparent;
        
        /** the diffuse texture id*/
        CTexture* diffuseTexture;
        
        /** the bump texture if available*/
        CTexture* bumpTexture;
        
        /** the specular texture if available*/
        CTexture* specularTexture;
        
        
        /** detail texture*/
        CTexture* detailTexture;
        
        /** detail factor*/
		float32 detailFactor;
        
        /** the cube map texture*/
        CTextureCubeMap* cubeMapTexture;
        
        /** the material index*/
        int32 materialIndex;
        
        
        /** constructor*/
        CMaterial()
        :
        ambient(vec4f(0.2f, 0.2f, 0.2f, 1.0f)),diffuse(vec4f(0.8f, 0.8f, 0.8f, 1.0f)),specular(vec4f(1.0f, 1.0f, 1.0f, 1.0f)),emissive(vec4f()),shininess(120.0f),opacity(1.0f), detailFactor(1.0),
        diffuseTexture(NULL),
        cubeMapTexture(NULL),
        bumpTexture(NULL),
        specularTexture(NULL),
        detailTexture(NULL),
        materialIndex(-1),
        Name(""),
        isTransparent(false),
        twoSided(0)
        {
            
        }
    private:
        /** copy constructor*/
        CMaterial(const CMaterial& m);
        
        /** = operator*/
        CMaterial& operator=(CMaterial const& m);
        
    };
}
#endif