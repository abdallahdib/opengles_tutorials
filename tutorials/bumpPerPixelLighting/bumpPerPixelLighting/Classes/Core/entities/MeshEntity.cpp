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
    
    for(uint32 g = 0; g < buffer->GroupsCount(); ++g)
	{
		CMeshGroup* grp = buffer->GroupAtIndex(g);
        CMaterial* material = buffer->MaterialForGroup(grp);
        
        //the following code can be merged with a renderer class
        //map material to shader context, this can be merged in a renderer class, where the renderer go through all material properties and set the appropriate GL state and shader uniform, for ex, bump texture, detail texture, (diffuse, ambient, specular properties ), face culling, depth write, transparent materials (blending) etc...
        if(material != NULL )
        {
            //send diffuse texture
            if( material->diffuseTexture != NULL && shader->texture0 != -1)
            {
                material->diffuseTexture->ActivateAndBind(GL_TEXTURE0);
                //shader->SetUniform1i("texture0", 0, shader->texture0);
                //or
                glUniform1i(shader->texture0, 0);
            }
            
            //send bump texture
            if(shader->textureBump != -1 && material->bumpTexture != NULL)
            {
                material->bumpTexture->ActivateAndBind(GL_TEXTURE1);
                //shader->SetUniform1i("textureBump", 1, shader->textureBump);
                //or
                glUniform1i(shader->textureBump, 1);
            }
            
            //send specular texture
            if(shader->textureSpecular != -1 && material->specularTexture != NULL)
            {
                material->specularTexture->ActivateAndBind(GL_TEXTURE2);
                //shader->SetUniform1i("textureBump", 1, shader->textureBump);
                //or
                glUniform1i(shader->textureSpecular, 2);
            }
            
            //send ambient
            if(shader->matColorAmbient != -1)
            {
                glUniform4fv(shader->matColorAmbient, 1, material->ambient);
            }
            
            //send diffuse
            if(shader->matColorDiffuse != -1)
            {
                glUniform4fv(shader->matColorDiffuse, 1, material->diffuse);
            }
            
            //send specular
            if(shader->matColorSpecular != -1)
            {
                glUniform4fv(shader->matColorSpecular, 1, material->specular);
            }
            
            //send shininess
            if(shader->matShininess != -1)
            {
                glUniform1f(shader->matShininess, material->shininess);
            }
            
        }
        
        grp->MapToGPU(0);
        glDrawElements(grp->GetDrawingMode(), grp->GetIndices().size(), GL_UNSIGNED_SHORT, 0);
    }

}