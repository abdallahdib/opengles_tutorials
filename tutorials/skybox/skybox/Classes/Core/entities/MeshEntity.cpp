/*
 *  MeshEntity.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "MeshEntity.h"

CMeshEntity::CMeshEntity(IMesh* mesh)
: m_pMesh(mesh)
{
    //m_pMesh = mesh->GetMesh();
}

CMeshEntity::~CMeshEntity()
{
    SAFE_DELETE(m_pMesh);
}

void CMeshEntity::Update(float32 dt)
{
}
void CMeshEntity::Render(CShader* shader, const mat4f &view, const mat4f &projection)
{
    assert ( shader != NULL );
    
    if(shader->matprojviewmodel != -1 )
    {
        mat4f mat = projection * view * m_mTransformationMatrix;
        glUniformMatrix4fv(shader->matprojviewmodel, 1, GL_FALSE, &mat[0]);
    }
    
    if(shader->matmodel != -1 )
    {
        glUniformMatrix4fv(shader->matmodel, 1, GL_FALSE, &m_mTransformationMatrix[0]);
    }
    
    CMeshBuffer* buffer = m_pMesh->GetMeshBuffer();
    
    //draw all grps in the mesh buffer
    for(uint32 g = 0; g < buffer->GroupsCount(); ++g)
	{
		CMeshGroup* grp = buffer->GroupAtIndex(g);
        CMaterial* material = buffer->MaterialForGroup(grp);
        
        //the following code can be merged with a renderer class
        //map material to shader context, this can be merged in a renderer class, where the renderer go through all material properties and set the appropriate GL state and shader uniform, for ex, bump texture, detail texture, (diffuse, ambient, specular properties ), face culling, depth write, transparent materials (blending) etc...
        if(material != NULL )
        {
            if( material->diffuseTexture != NULL && shader->texture0 != -1)
            {
                material->diffuseTexture->ActivateAndBind(GL_TEXTURE0);
                shader->SetUniform1i("texture0", 0, shader->texture0);
            }
            
            //send detail texture with the corresponding detail factor of the material
            if(material->detailTexture !=0)
            {
                material->detailTexture->ActivateAndBind(GL_TEXTURE1);
                glUniform1i(shader->textureDetail, 1);
                glUniform1f(shader->detailFactor, material->detailFactor);
            }
        }
        
        //bind vao
        grp->MapToGPU(0);
        
        //issue gl drawing command
        glDrawElements(grp->GetDrawingMode(), grp->GetIndices().size(), GL_UNSIGNED_SHORT, 0);
    }

}