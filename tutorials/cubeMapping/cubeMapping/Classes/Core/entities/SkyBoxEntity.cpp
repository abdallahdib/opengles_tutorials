/*
 *  SkyBoxEntity.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "SkyBoxEntity.h"

CSkyBoxEntity::CSkyBoxEntity(float32 size,  const vec3f& position)
:
m_fSize(size)
{
    m_pMeshBuffer = new CMeshBuffer();
    m_mTransformationMatrix =mat4f::createTranslation(position.x, position.y, position.z);
}
CSkyBoxEntity::~CSkyBoxEntity()
{
    SAFE_DELETE( m_pMeshBuffer );
}
bool CSkyBoxEntity::Create(CTextureCubeMap* texture_cubeMap)
{
    CMeshGroup* group = new CMeshGroup();
    std::vector<CGpuVertex> &vert = group->GetVertices();
    std::vector<ushort16> &ind = group->GetIndices();
    group->SetMaterialIndex(0);

    CGpuVertex vertex;
    vertex.normal = vec3f(0.0f, 0.0f, 0.0f);
    vertex.texCoord = vec2f(0.0f, 0.0f);
    
    float32 t = m_fSize;
    vertex.pos = vec3f(-t, -t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t, -t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t, -t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t, -t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t,  t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t,  t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t,  t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t,  t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t, -t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t,  t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t,  t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t, -t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t, -t, t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t,  t, t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t,  t, t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t, -t, t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t, -t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t, -t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t,  t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(-t,  t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t, -t, -t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t, -t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t,  t,  t);
    vert.push_back(vertex);
    
    vertex.pos = vec3f(t,  t, -t);
    vert.push_back(vertex);
    
    ind.push_back((ushort16)0);
    ind.push_back((ushort16)2);
    ind.push_back((ushort16)1);
    
    ind.push_back((ushort16)0);
    ind.push_back((ushort16)3);
    ind.push_back((ushort16)2);
    
    ind.push_back((ushort16)4);
    ind.push_back((ushort16)5);
    ind.push_back((ushort16)6);
    
    ind.push_back((ushort16)4);
    ind.push_back((ushort16)6);
    ind.push_back((ushort16)7);
    
    ind.push_back((ushort16)8);
    ind.push_back((ushort16)9);
    ind.push_back((ushort16)10);
    
    ind.push_back((ushort16)8);
    ind.push_back((ushort16)10);
    ind.push_back((ushort16)11);
    
    ind.push_back((ushort16)12);
    ind.push_back((ushort16)15);
    ind.push_back((ushort16)14);
    
    ind.push_back((ushort16)12);
    ind.push_back((ushort16)14);
    ind.push_back((ushort16)13);
    
    ind.push_back((ushort16)16);
    ind.push_back((ushort16)17);
    ind.push_back((ushort16)18);
    
    ind.push_back((ushort16)16);
    ind.push_back((ushort16)18);
    ind.push_back((ushort16)19);
    
    ind.push_back((ushort16)20);
    ind.push_back((ushort16)23);
    ind.push_back((ushort16)22);
    
    ind.push_back((ushort16)20);
    ind.push_back((ushort16)22);
    ind.push_back((ushort16)21);
    
    //create gpu storage for the gourp
    group->AllocateOnGpuMemory(kGL_BUFFER_USAGE_HINT_STATIC);
    
    //create a bounding box for the group
    group->CreateBoundingBox();
    
    //create skybox material
    CMaterial *mat = new CMaterial;
    mat->materialIndex = 0;
    
    if(texture_cubeMap != NULL)
        mat->cubeMapTexture = texture_cubeMap;
    
    mat->Name = "Material0";
    
    //disable face culling
    mat->twoSided = 1;
    
    //add group/material to buffer
    m_pMeshBuffer->AddGroup(group);
    m_pMeshBuffer->AddMaterial(mat);
    
    return true;
}

void CSkyBoxEntity::Render(CShader* shader, const mat4f &view, const mat4f &projection)
{
    assert ( shader != NULL && m_pMeshBuffer != NULL);
    
    if(shader->matprojviewmodel != -1 )
    {
        mat4f mat = projection * view * m_mTransformationMatrix;
        glUniformMatrix4fv(shader->matprojviewmodel, 1, GL_FALSE, &mat[0]);
    }
    
    for(uint32 g = 0; g<m_pMeshBuffer->GroupsCount(); g++)
	{
		CMeshGroup* grp = m_pMeshBuffer->GroupAtIndex(g);
        CMaterial* material = m_pMeshBuffer->MaterialForGroup(grp);
        
        //the following code can be merged with a renderer class
        //map material to shader context, this can be merged in a renderer class, where the renderer go through all material properties and set the appropriate GL state and shader uniform, for ex, bump texture, detail texture, (diffuse, ambient, specular properties ), face culling, depth write, transparent materials (blending) etc...
        if( material != NULL && material->cubeMapTexture != NULL && shader->textureCubeMap != -1)
		{
			material->cubeMapTexture->ActivateAndBind(GL_TEXTURE2);
			//shader->SetUniform1i("textureCubeMap", 2, shader->textureCubeMap);
            //or
            glUniform1i(shader->textureCubeMap, 2);

        }
        
        //todo: u have to check for other material properties and set GL state accordingly. twosided for face culling, depth mask ...
        
        //bind vertex array
        grp->MapToGPU(0);
        glDrawElements(grp->GetDrawingMode(), grp->GetIndices().size(), GL_UNSIGNED_SHORT, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    }
}


