/*
 *  AssimpMesh.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */


#include "AssimpMesh.h"
#include "Helpers.h"
#include "CacheResourceManager.h"

namespace vvision
{
    
    CAssimpMesh::CAssimpMesh()
    :m_pAssimpScene(NULL), m_sMeshName(""), m_pAnimator(NULL)
    {
    }
    CAssimpMesh::~CAssimpMesh()
    {
        //managed by the resource manager
        m_pAssimpScene = NULL;
        
        if(m_pAnimator != NULL)
        {
            delete m_pAnimator;
            m_pAnimator = NULL;
        }
    }
    bool CAssimpMesh::LoadMesh(const std::string& FileName)
    {
        m_sMeshName = FileName;
        
        CCacheResourceManager& res = CCacheResourceManager::Instance();
        m_pAssimpScene = (aiScene*)res.LoadMesh(FileName);
        
        if(m_pAssimpScene == NULL)
            return false;
        
        // extract mesh groups
        for (uint32 i = 0 ; i < m_pAssimpScene->mNumMeshes ; i++)
        {
            const aiMesh* paiMesh = m_pAssimpScene->mMeshes[i];
            
            if(!ExtractMeshGroup(i, paiMesh))
                return false;
            
        }
        
        //extract materials ( groups can share materials
        bool ret = ExtractMaterials(m_pAssimpScene);
        
        //extract animations
        if(m_pAssimpScene->HasAnimations())
        {
            m_pAnimator = new AssimpSceneAnimator(m_pAssimpScene);
        }
        
        return ret;
        
    }
    
    void CAssimpMesh::DeleteCachedResources()
    {
        CCacheResourceManager::Instance().DeleteResource(kRESOURCE_TYPE_MESH, m_sMeshName);
        
        if(m_pMeshBuffer != NULL)
        {
            std::vector<CMaterial*>& materials = m_pMeshBuffer->GetMaterialsContainerRef();
            
            for(std::vector<CMaterial*>::iterator it = materials.begin(); it != materials.end(); ++it)
            {
                if( (*it)->diffuseTexture != NULL)
                {
                    if( CCacheResourceManager::Instance().DeleteResourceByMemoryAddress(kRESOURCE_TYPE_TEXTURE2D, (*it)->diffuseTexture) )
                        (*it)->diffuseTexture = NULL;
                }
                
                if( (*it)->bumpTexture != NULL)
                {
                    if( CCacheResourceManager::Instance().DeleteResourceByMemoryAddress(kRESOURCE_TYPE_TEXTURE2D, (*it)->bumpTexture) )
                        (*it)->bumpTexture = NULL;
                }
                
                if( (*it)->detailTexture != NULL)
                {
                    if( CCacheResourceManager::Instance().DeleteResourceByMemoryAddress(kRESOURCE_TYPE_TEXTURE2D, (*it)->detailTexture) )
                        (*it)->detailTexture = NULL;
                }
                
                if( (*it)->cubeMapTexture != NULL)
                {
                    if( CCacheResourceManager::Instance().DeleteResourceByMemoryAddress(kRESOURCE_TYPE_TEXTURE2D, (*it)->cubeMapTexture) )
                        (*it)->cubeMapTexture = NULL;
                }
            }
        }
        
        m_pAssimpScene = NULL;
    }
    
    bool CAssimpMesh::ExtractMeshGroup(uint32 Index, const aiMesh* paiMesh)
    {
        CMeshGroup *group = new CMeshGroup();
        group->SetMaterialIndex(paiMesh->mMaterialIndex );
        
        bool hasTangents = paiMesh->HasTangentsAndBitangents();
        bool hasNormals = paiMesh->HasNormals();
        
   
        std::vector<CGpuVertex> &Vertices = group->GetVertices();
        std::vector<ushort16> &Indices = group->GetIndices();
        
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
        
        //collect weights on all vertices
        std::vector<std::vector<aiVertexWeight> > weightsPerVertex(paiMesh->mNumVertices);
        for (uint32 a = 0; a < paiMesh->mNumBones; a++) {
            const aiBone* bone = paiMesh->mBones[a];
            
            if(bone == NULL)
                continue;
            
            for (uint32 b = 0; b<bone->mNumWeights; b++) {
                weightsPerVertex[bone->mWeights[b].mVertexId].push_back(aiVertexWeight(a, bone->mWeights[b].mWeight));
                
            }
        }
        
        //extract vertex attributes ( pos, normal, tex coord, bone indices and weights)
        for (uint32 i = 0 ; i < paiMesh->mNumVertices ; i++)
        {
            //extract vertex pos normal texture coordinates (actually tangent are not extracted)
            const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
            const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
            const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
            const aiVector3D* pTangCoord = &(paiMesh->mTangents[i]);
            
            //fill vertex attributes
            CGpuVertex v;
            
            if(pPos != NULL)
                v.pos = vec3f(pPos->x, pPos->y, pPos->z);
            
            if(pTexCoord != NULL)
                v.texCoord = vec2f(pTexCoord->x, pTexCoord->y);
            
            if(hasNormals && pNormal != NULL)
                v.normal = vec3f(pNormal->x, pNormal->y, pNormal->z);
            
            if(hasTangents && pTangCoord != NULL)
                v.tangent = vec3f(pTangCoord->x, pTangCoord->y, pTangCoord->z);
            
            //extract bone indices and weights
            if(paiMesh->HasBones())
            {
                //assert(weightsPerVertex[i].size() <= 4);
                for(uint32 a = 0; a < weightsPerVertex[i].size(); a++)
                {
                    //fill vertex attributes
                    v.boneIndices[a] = (float32)weightsPerVertex[i][a].mVertexId;
                    v.boneWeights[a] = weightsPerVertex[i][a].mWeight /** 255.f*/;
                }
            }
            
            //push vertex into buffer
            Vertices.push_back(v);
        }
        
        //extract indices
        for (uint32 i = 0 ; i < paiMesh->mNumFaces ; i++) {
            const aiFace& Face = paiMesh->mFaces[i];
            //assert(Face.mNumIndices == 3);
            Indices.push_back((ushort16)Face.mIndices[0]);
            Indices.push_back((ushort16)Face.mIndices[1]);
            Indices.push_back((ushort16)Face.mIndices[2]);
        }
        
        //allocate buffer on gpu
        if(!group->AllocateOnGpuMemory(kGL_BUFFER_USAGE_HINT_STATIC))
        {
            delete group;
            return false;
        }
        
        //create mesh bounding box
        group->CreateBoundingBox();
        
        //push mesh group
        m_pMeshBuffer->AddGroup(group);
        
        return true;
    }
    
    bool CAssimpMesh::ExtractMaterials(const aiScene* pScene)
    {
        CCacheResourceManager& res = CCacheResourceManager::Instance();
        
        // Initialize the materials
        for (uint32 i = 0 ; i < pScene->mNumMaterials ; i++) {
            CMaterial* material = new CMaterial();
            const aiMaterial* pMaterial = pScene->mMaterials[i];
            
            material->diffuseTexture = NULL;
            
            if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString path;
                
                if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                    material->diffuseTexture = (CTexture*)res.LoadTexture2D(path.data);
                }
            }
            
            struct aiColor4D specular, diffuse, ambient;
            vec4f zero;
            
            //diffuse
            if((AI_SUCCESS == aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse)))
                material->diffuse = vec4f(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
            else
                material->diffuse = zero;
            
            //ambiant
            if((AI_SUCCESS == aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient)))
                material->ambient = vec4f(ambient.r, ambient.g, ambient.b, ambient.a);
            else 
                material->ambient = zero;	
            
            //specular
            if((AI_SUCCESS == aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &specular)))
                material->specular = vec4f(specular.r, specular.g, specular.b, specular.a);
            else 
                material->specular = zero;	
            
            //shininess
            aiGetMaterialFloat(pMaterial,AI_MATKEY_SHININESS,&material->shininess);
            if(material->shininess <1.0f)
                material->shininess = 15;
            
            aiGetMaterialFloat(pMaterial,AI_MATKEY_OPACITY,&material->opacity);
            if(material->opacity< 1.f)
                material->isTransparent = true;
            
            aiGetMaterialInteger(pMaterial,AI_MATKEY_TWOSIDED,&material->twoSided);
            m_pMeshBuffer->AddMaterial(material);
        }
        return true;
    }
}



