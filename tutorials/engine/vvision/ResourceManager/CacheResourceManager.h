/* CacheResourceManager.h
 *
 * Virtual Vision Engine . Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/



#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Singleton.h"
#include "Texture.h"
#include "TextureCubeMap.h"
#include "Shader.h"
#include "assimp.hpp"      // C++ importer interface
#include "aiScene.h"       // Output data structure
#include "aiPostProcess.h" // Post processing flags
#include "types.h"

namespace vvision
{
    /** resource type, texture, shader or mesh*/
    enum RESOURCE_TYPE
    {
        /** texture 2D*/
        kRESOURCE_TYPE_TEXTURE2D,
        
        /** texture cube map resource*/
        kRESOURCE_TYPE_TEXTURE_CUBE_MAP,
        
        /** shader resource*/
        kRESOURCE_TYPE_SHADER,
        
        /** mesh resource*/
        kRESOURCE_TYPE_MESH
    };
    
    CREATE_SINGLETON( CCacheResourceManager )
    
    public :
    
    /** load texture, , first it search the cache for any exisiting resource with the same name if not found it loads and cache it , returns NULL if not found*/
    CTexture* LoadTexture2D(const std::string& name,
                            bool buildMipMapTexture=true,
                            GLint wrap_s=GL_REPEAT,
                            GLint wrap_t=GL_REPEAT,
                            GLint wrap_r=GL_REPEAT,
                            GLint mag_filter=GL_LINEAR,
                            GLint min_filter=GL_LINEAR_MIPMAP_LINEAR
                            );
    
    /** load texture, first it will search the cache for any exisiting resource with the same name if not found it loads and cache it  , returns NULL if not found*/
    CTextureCubeMap* LoadTextureCube(const std::string& name,
                                     bool buildMipMapTexture=true,
                                     GLint wrap_s=GL_REPEAT,
                                     GLint wrap_t=GL_REPEAT,
                                     GLint wrap_r=GL_REPEAT,
                                     GLint mag_filter=GL_LINEAR,
                                     GLint min_filter=GL_LINEAR_MIPMAP_LINEAR
                                     );
    
    /** load shader, first it search the cache for any exisiting resource with the same name if not found it loads and cache it  , returns NULL if not found */
    CShader* LoadShader(const std::string& name);
    
    /** load mesh, first it search the cache for any exisiting resource with the same name if not found it loads and cache it , returns NULL if not found */
    const aiScene* LoadMesh(const std::string& name);
    
    /** remove and delete resource from cache, this will invoke delete operator on the resource*/
    bool DeleteResource(RESOURCE_TYPE type, const std::string& name);
    
    /** remove and delete resource from cache, this will invoke delete operator on the resource*/
    bool DeleteResourceByMemoryAddress(RESOURCE_TYPE type, void* pointer);
    
    /** empty cache, this will delete all cached resources that has been already loaded by the resource manager*/
    void Destroy();
    
private:
    
    /** not allowed*/
    ~CCacheResourceManager()
    {
        Destroy();
    }
    
    /** not allowed*/
    CCacheResourceManager()
    {
    }
    
    /** not allowed*/
    CCacheResourceManager(const CCacheResourceManager& r);
    
    /** not allowed*/
    CCacheResourceManager& operator=(const CCacheResourceManager& r);
    
    /** assimp mesh importer*/
    Assimp::Importer Importer;
    
    /** array of cached mesh resources*/
    std::map<std::string, aiScene*> meshResources;
    
    /** array to cached texture 2D */
    std::map<std::string, CTexture*> tex2dResources;
    
    /** array to cached texture cube map */
    std::map<std::string, CTextureCubeMap*> texCubeResources;
    
    /** array of chaced shaders*/
    std::map<std::string, CShader*> shaderResources;
    
};
}
#endif
