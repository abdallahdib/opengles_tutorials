/*
 *  CacheResourceManager.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "CacheResourceManager.h"
#include "Helpers.h"
#include "FilePath.h"
namespace vvision
{
    void CCacheResourceManager::Destroy()
    {
        //texture cleanup
        for(std::map<std::string, CTexture*>::iterator it = tex2dResources.begin(); it != tex2dResources.end(); it++) {
            if( (*it).second != NULL)
            {
                delete (*it).second;
                (*it).second = NULL;
            }
        }
        tex2dResources.clear();
        
        //shader cleanup
        for(std::map<std::string, CShader*>::iterator it = shaderResources.begin(); it != shaderResources.end(); it++) {
            if( (*it).second != NULL)
            {
                delete (*it).second;
                (*it).second = NULL;
            }
        }
        shaderResources.clear();
        
        //cube map resources
        for(std::map<std::string, CTextureCubeMap*>::iterator it = texCubeResources.begin(); it != texCubeResources.end(); it++) {
            if( (*it).second != NULL)
            {
                delete (*it).second;
                (*it).second = NULL;
            }
        }
        texCubeResources.clear();
        
        //aimesh
        for(std::map<std::string, aiScene*>::iterator it = meshResources.begin(); it != meshResources.end(); it++) {
            if( (*it).second != NULL)
            {
                delete (*it).second;
                (*it).second = NULL;
            }
        }
        meshResources.clear();
    }
    
    CTexture* CCacheResourceManager::LoadTexture2D(const std::string& name,
                                                   bool buildMipMapTexture,
                                                   GLint wrap_s,
                                                   GLint wrap_t,
                                                   GLint wrap_r,
                                                   GLint mag_filter,
                                                   GLint min_filter
                                                   )
    {
        
        if(tex2dResources.find(name) != tex2dResources.end())
        {
#ifdef DEBUG
            std::cerr<<"CCacheResourceManager: [INFO] "<< name<<" loaded from cache.\n";
#endif
            return tex2dResources.find(name)->second;
        }
        
        int32 width,height;
        char8 * data = LoadImage(getPath(name.c_str()), &width, &height);
        
        if(data == NULL)
        {
            //#ifdef DEBUG
            std::cerr<< "CCacheResourceManager: [ERROR] could not load texture: "<< name << "...<\n";
            //#endif
            return NULL;
        }
        
        CTexture* ptr=new CTexture();
        ptr->CreateTexture(GL_TEXTURE_2D,
                           width, height, GL_RGBA,
                           GL_UNSIGNED_BYTE, GL_RGBA, (uchar8*)data,
                           wrap_s, wrap_t,
                           min_filter, mag_filter, buildMipMapTexture
                           );
        free(data);
        tex2dResources[name] = ptr;
#ifdef DEBUG
        std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name <<" texture loaded...\n";
#endif
        return ptr;
        
    }
    
    CTextureCubeMap* CCacheResourceManager::LoadTextureCube(const std::string& name,
                                                            bool buildMipMapTexture,
                                                            GLint wrap_s,
                                                            GLint wrap_t,
                                                            GLint wrap_r,
                                                            GLint mag_filter,
                                                            GLint min_filter
                                                            )
    {
        
        //resource already loaded !
        if(texCubeResources.find(name) != texCubeResources.end())
        {
#ifdef DEBUG
            std::cerr<<"CCacheResourceManager: [INFO] "<< name<<" loaded from cache.\n";
#endif
            return texCubeResources.find(name)->second;
        }
        
#define CCRM_CUBE_MAP_NOT_FOUND \
std::cerr<< "CCacheResourceManager: [ERROR] could not load texture cube map: "<< name << "...<\n";
        
        int32 width,height;
        string extension, base;
        StringManipulator::GetExtensitonType(name, extension);
        StringManipulator::GetBaseName(name, base);
        ostringstream os;
        
        //xp
        os.str("");
        os<<base<<"_xp."<<extension;
        char8 * data_xp = LoadImage(getPath(os.str()), &width, &height);
        
        if(data_xp == NULL)
        {
            CCRM_CUBE_MAP_NOT_FOUND
            return NULL;
        }
        
        //xn
        os.str("");
        os<<base<<"_xn."<<extension;
        char8 * data_xn = LoadImage(getPath(os.str()), &width, &height);
        
        if(data_xn == NULL)
        {
            CCRM_CUBE_MAP_NOT_FOUND
            free(data_xp);
            return NULL;
        }
        
        //yp
        os.str("");
        os<<base<<"_yp."<<extension;
        char8 * data_yp = LoadImage(getPath(os.str()), &width, &height);
        
        if(data_yp == NULL)
        {
            CCRM_CUBE_MAP_NOT_FOUND
            free(data_xp);
            free(data_xn);
            return NULL;
        }
        
        //yn
        os.str("");
        os<<base<<"_yn."<<extension;
        char8 * data_yn = LoadImage(getPath(os.str()), &width, &height);
        
        if(data_yn == NULL)
        {
            CCRM_CUBE_MAP_NOT_FOUND
            free(data_xp);
            free(data_xn);
            free(data_yp);
            return NULL;
        }
        
        //zp
        os.str("");
        os<<base<<"_zp."<<extension;
        char8 * data_zp = LoadImage(getPath(os.str()), &width, &height);
        
        if(data_zp == NULL)
        {
            CCRM_CUBE_MAP_NOT_FOUND
            free(data_xp);
            free(data_xn);
            free(data_yp);
            free(data_yn);
            return NULL;
        }
        
        //yn
        os.str("");
        os<<base<<"_zn."<<extension;
        char8 * data_zn = LoadImage(getPath(os.str()), &width, &height);
        
        if(data_zn == NULL)
        {
            CCRM_CUBE_MAP_NOT_FOUND
            free(data_xp);
            free(data_xn);
            free(data_yp);
            free(data_yn);
            free(data_zp);
            return NULL;
        }
        
        CTextureCubeMap* ptr=new CTextureCubeMap();
        ptr->CreateTexture(GL_TEXTURE_CUBE_MAP,
                           width, height, GL_RGBA,
                           GL_UNSIGNED_BYTE, GL_RGBA,
                           (uchar8*)data_xp,
                           (uchar8*)data_xn,
                           
                           (uchar8*)data_yp,
                           (uchar8*)data_yn,
                           
                           (uchar8*)data_zp,
                           (uchar8*)data_zn,
                           
                           wrap_s, wrap_t,
                           mag_filter, min_filter, buildMipMapTexture
                           );
        free(data_xp);free(data_xn);
        free(data_yp);free(data_yn);
        free(data_zp);free(data_zn);
        
        //cache resource
        texCubeResources[name] = ptr;
#ifdef DEBUG
        std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name<<" texture cube map loaded...\n";
#endif
        return ptr;
        
    }
    
    CShader* CCacheResourceManager::LoadShader(const std::string& name)
    {
        
        if(shaderResources.find(name) != shaderResources.end())
        {
#ifdef DEBUG
            std::cerr<<"CCacheResourceManager: [INFO] "<< name<<" loaded from cache.\n";
#endif
            return shaderResources.find(name)->second;
        }
        
        
        CShader* ptr = new CShader();
        string vshader = name;
        string pshader = name;
        StringManipulator::AddCharArrayToString(vshader, ".vsh");
        StringManipulator::AddCharArrayToString(pshader, ".fsh");
        
        if(!ptr->LoadShadersFromMemory(getContentFromPath(getPath(vshader)).c_str(),
                                       getContentFromPath(getPath(pshader)).c_str()))
        {
            std::cerr<< "CCacheResourceManager: [ERROR] failed to load shader: "<< name << "...<\n";
            delete ptr;
            return NULL;
        }
        
        shaderResources[name] = ptr;
        
#ifdef DEBUG
        std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name <<" shader loaded...\n";
#endif
        
        return ptr;
    }
    
    const aiScene* CCacheResourceManager::LoadMesh(const std::string& name)
    {
        
        if(meshResources.find(name) != meshResources.end())
        {
#ifdef DEBUG
            std::cerr<<"CCacheResourceManager: [INFO] "<< name<<" loaded from cache.\n";
#endif
            return meshResources.find(name)->second;
        }
        
        // todo: we must be able to customize this
        uint32 ppsteps =
        //xaiProcess_CalcTangentSpace      | // calculate tangents and bitangents if possible
        aiProcess_CalcTangentSpace         | //calculate tangents if possible
        aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
        aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
        aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
        aiProcess_RemoveRedundantMaterials | // remove redundant materials
        aiProcess_FindDegenerates          | // remove degenerated polygons from the import
        aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
        aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
        //aiProcess_TransformUVCoords      | // preprocess UV transformations (scaling, translation ...)
        aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
        aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
        aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
        aiProcess_SplitByBoneCount         | // split meshes with too many bones. Necessary for our (limited) hardware
        aiProcess_FlipUVs                  | // flip uv tex coord
        aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
        aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
        aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
        // aiProcess_ConvertToLeftHanded   | // convert everything to D3D left handed space
        aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives
        aiProcess_OptimizeGraph            |
        0;
        Importer.ReadFile(getPath(name).c_str(), ppsteps );
        
        aiScene* ptr = Importer.GetOrphanedScene();
        
        if(!ptr)
        {
            std::cerr<< "CCacheResourceManager: [ERROR] could not load mesh:"<< name <<". Reason: "<<Importer.GetErrorString()<<"\n";
            return NULL;
        }
        meshResources[name] = ptr;
        
#ifdef DEBUG
        std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name <<" mesh loaded...\n";
#endif
        
        return ptr;
    }
    
    bool CCacheResourceManager::DeleteResource(RESOURCE_TYPE type, const std::string& name)
    {
        switch (type)
        {
            case kRESOURCE_TYPE_TEXTURE2D:
            {
                std::map<std::string, CTexture*>::iterator itT;
                itT = tex2dResources.find(name);
                
                if(itT != tex2dResources.end())
                {
                    delete (itT->second);
                    tex2dResources.erase(itT);
#ifdef DEBUG
                    std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name <<" texture removed from cache...\n";
#endif
                    return true;
                }
            }
                break;
                
            case kRESOURCE_TYPE_TEXTURE_CUBE_MAP:
            {
                std::map<std::string, CTextureCubeMap*>::iterator itCM;
                itCM = texCubeResources.find(name);
                
                if(itCM != texCubeResources.end())
                {
                    texCubeResources.erase(itCM);
                    delete (itCM->second);
#ifdef DEBUG
                    std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name <<" texture cube map removed from cache...\n";
#endif
                    return true;
                }
            }
                break;
                
            case kRESOURCE_TYPE_SHADER:
            {
                std::map<std::string, CShader*>::iterator itSH;
                itSH = shaderResources.find(name);
                
                if(itSH != shaderResources.end())
                {
                    shaderResources.erase(itSH);
                    delete (itSH->second);
#ifdef DEBUG
                    std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name <<" shader removed from cache...\n";
#endif
                    return true;
                }
            }
                break;
                
            case kRESOURCE_TYPE_MESH:
            {
                std::map<std::string, aiScene*>::iterator itME;
                itME = meshResources.find(name);
                
                if(itME != meshResources.end())
                {
                    meshResources.erase(itME);
                    delete (itME->second);
#ifdef DEBUG
                    std::cerr<< "CCacheResourceManager: [SUCCESS] "<<name <<" mesh removed from cache...\n";
#endif
                    return true;
                }
            }
                break;
                
            default:
                break;
        }
#ifdef DEBUG
        std::cerr<< "CCacheResourceManager: [ERROR] cache resource not found for "<< name << "...<\n";
#endif
        return false;
    }
    
    bool CCacheResourceManager::DeleteResourceByMemoryAddress(RESOURCE_TYPE type, void* pointer)
    {
        if(pointer == NULL)
            return false;
        
        //todo refactor
        switch (type)
        {
            case kRESOURCE_TYPE_TEXTURE2D:
            {
                
                for( std::map<std::string, CTexture*>::iterator itT = tex2dResources.begin(); itT != tex2dResources.end(); ++itT)
                {
                    if(itT->second == pointer)
                    {
#ifdef DEBUG
                        std::cerr<< "CCacheResourceManager: [SUCCESS] "<<itT->first <<" removed from cache...\n";
#endif
                        delete (itT->second);
                        tex2dResources.erase(itT);
                        return true;
                    }
                }
            }
                
                break;
                
            case kRESOURCE_TYPE_TEXTURE_CUBE_MAP:
            {
                
                for( std::map<std::string, CTextureCubeMap*>::iterator itCM = texCubeResources.begin(); itCM != texCubeResources.end(); ++itCM)
                {
                    if(itCM->second == pointer)
                    {
#ifdef _DEBUG
                        std::cerr<< "CCacheResourceManager: [SUCCESS] "<< itCM->first<<" removed from cache.\n";
#endif
                        delete (itCM->second);
                        texCubeResources.erase(itCM);
                        
                        return true;
                    }
                }
            }
                break;
                
            case kRESOURCE_TYPE_SHADER:
            {
                for( std::map<std::string, CShader*>::iterator itSH = shaderResources.begin(); itSH != shaderResources.end(); ++itSH)
                {
                    if(itSH->second == pointer)
                    {
#ifdef DEBUG	                        
                        std::cerr<< "CCacheResourceManager: [SUCCESS] "<<itSH->first <<" removed from cache...\n";
#endif   
                        delete (itSH->second);
                        shaderResources.erase(itSH);
                        return true;
                    }
                    
                }
            }
                break;
                
            case kRESOURCE_TYPE_MESH:
            {
                for( std::map<std::string, aiScene*>::iterator itME = meshResources.begin(); itME != meshResources.end(); ++itME)
                {
                    if(itME->second == pointer)
                    {
#ifdef DEBUG	                        
                        std::cerr<< "CCacheResourceManager: [SUCCESS] "<<itME->first <<" removed from cache...\n";
#endif 
                        delete (itME->second);
                        meshResources.erase(itME);
                        return true;
                    }
                    
                }
                
            }
                break;
                
            default:
                break;
        }
        
#ifdef DEBUG	
        std::cerr<< "CCacheResourceManager: [ERROR] could not delete resource for"<< pointer << ". not found in cache...\n";
#endif  
        return false;
    }
}

