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
    
    //send modelViewprojection matrix to the current shader
    if(shader->matprojviewmodel != -1 )
    {
        mat4f mat = projection * view * m_mTransformationMatrix;
        glUniformMatrix4fv(shader->matprojviewmodel, 1, GL_FALSE, &mat[0]);
    }
    
    //send model matrix if the shader needs
    if(shader->matmodel != -1 )
    {
        glUniformMatrix4fv(shader->matmodel, 1, GL_FALSE, &m_mTransformationMatrix[0]);
    }
    
    //this part can be merged with a renderer class
    //get the mesh buffer
    CMeshBuffer* buffer = m_pMesh->GetMeshBuffer();
    
    //go through all groups in the mesh buffer
    for(uint32 g = 0; g < buffer->GroupsCount(); ++g)
	{
        //render each group
		CMeshGroup* grp = buffer->GroupAtIndex(g);
        
        //get corresponding material for each group
        CMaterial* material = buffer->MaterialForGroup(grp);
        
        //the following code can be merged with a renderer class
        //map material to shader context, this can be merged in a renderer class, where the renderer go through all material properties and set the appropriate GL state and shader uniform, for ex, bump texture, detail texture, (diffuse, ambient, specular properties ), face culling, depth write, transparent materials (blending) etc...
        if(material != NULL )
        {
            if( material->diffuseTexture != NULL && shader->texture0 != -1)
            {
                material->diffuseTexture->ActivateAndBind(GL_TEXTURE0);
                //shader->SetUniform1i("texture0", 0, shader->texture0);
                //or
                glUniform1i(shader->texture0, 0);
            }
        }
        
        //enable vertex array object (vao)
        grp->MapToGPU(0);
        
        //perform GL draw for each group
        glDrawElements(grp->GetDrawingMode(), grp->GetIndices().size(), GL_UNSIGNED_SHORT, 0);
    }

}