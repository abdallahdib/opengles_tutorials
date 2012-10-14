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
    
    //this part can be merged in a renderer class
    //go through all groups in the buffer and draw them one by one
    for(uint32 g = 0; g < buffer->GroupsCount(); ++g)
	{
		CMeshGroup* grp = buffer->GroupAtIndex(g);
        //get the material for current material
        CMaterial* material = buffer->MaterialForGroup(grp);
        
        if(material != NULL )
        {
            if( material->diffuseTexture != NULL && shader->texture0 != -1)
            {
                //Enable(TEXTURING_2D);
                material->diffuseTexture->ActivateAndBind(GL_TEXTURE0);
                glUniform1i(shader->texture0, 0);
            }
            
            /*
             //no bump in this tutorial
            if(shader->textureBump != -1 && material->bumpTexture != NULL)
            {
                material->bumpTexture->ActivateAndBind(GL_TEXTURE1);
                shader->SetUniform1i("textureBump", 1, shader->textureBump);
            }*/
            
            if(material->detailTexture !=0 && material->detailTexture != NULL)
            {
                material->detailTexture->ActivateAndBind(GL_TEXTURE2);
                glUniform1i(shader->textureDetail, 2);
                glUniform1f(shader->detailFactor, material->detailFactor);
            }
                
        }
        
        grp->MapToGPU(0);
        glDrawElements(grp->GetDrawingMode(), grp->GetIndices().size(), GL_UNSIGNED_SHORT, 0);
    }

}