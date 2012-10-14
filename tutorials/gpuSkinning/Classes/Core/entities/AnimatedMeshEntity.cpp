/*
 *  AnimatedMeshEntity.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "AnimatedMeshEntity.h"
#include "AssimpSceneAnimator.h"

CAnimatedMeshEntity::CAnimatedMeshEntity(CAssimpMesh* mesh)
: m_pMesh(mesh)
{
    m_pAnimator = m_pMesh->GetAnimator();
}

CAnimatedMeshEntity::~CAnimatedMeshEntity()
{
    SAFE_DELETE(m_pMesh);
}
void CAnimatedMeshEntity::Update(float32 dt)
{
    m_pAnimator->Calculate(dt);
}
void CAnimatedMeshEntity::RenderNode(aiNode* piNode)
{    
    CMeshBuffer* meshBuffer = m_pMesh->GetMeshBuffer();

    for (int32 i = 0; i < piNode->mNumMeshes; ++i)
    {
        const aiMesh* mesh = m_pAnimator->mScene->mMeshes[piNode->mMeshes[i]];
        
        if( mesh->HasBones())
        {
            //get bone matrices for the current frame, this bone matrices holds the transformation matrix for each bone.
            //each vertex position is affected by certain number of bones,
            //vertex attributes holds the index of influenced bone and the corresponding weight.
            //for eg: a vertex 'vert' is influenced by 'bone0' with 'weight0' and 'bone1' with 'weight1'.
            //the final position of 'vert' is:
            //vert.finalpos = weight0 * (bone0.matrix * vert.pos) + weight1 * (bone1.matrix * vert.pos);
            std::vector<aiMatrix4x4>& matRef = (std::vector<aiMatrix4x4>& )m_pAnimator->GetBoneMatrices(piNode, i);
            
            for(int32 i = 0; i< matRef.size(); i++)
                matRef[i].Transpose();
            
            //send the bones transformations at time t to he shader to pefrom the skinning
            m_pShader->SetUniformMatrix4x4fv("skinningMatrix", matRef.size(), GL_FALSE, &matRef[0].a1);
            
            //our importer presever the same grp order
            CMeshGroup* grp = meshBuffer->GroupAtIndex(i);
            
            //get material
            CMaterial* material = meshBuffer->MaterialForGroup(grp);

            //the following code can be merged with a renderer class
            //map material to shader context, we only have diffuse texture, this can be merged in a renderer class, where the renderer go through all material properties and set the appropriate GL state and shader uniform, for ex, bump texture, detail texture, (diffuse, ambient, specular properties ), face culling, depth write, transparent etc...
            if( material != NULL && material->diffuseTexture != NULL && m_pShader->texture0 != -1)
            {
                material->diffuseTexture->ActivateAndBind(GL_TEXTURE0);
                m_pShader->SetUniform1i("texture0", 0, m_pShader->texture0);
            }
            
            //this will only enable vertex array object (vao)
            grp->MapToGPU(0);
            
            //render grp
            glDrawElements(grp->GetDrawingMode(), grp->GetIndices().size(), GL_UNSIGNED_SHORT, 0);
            
        }
    }
    
    //apply for children 
    for (int32 i = 0; i < piNode->mNumChildren;++i)
        RenderNode(piNode->mChildren[i]);
    
}
void CAnimatedMeshEntity::Render(CShader* shader,  const mat4f &view, const mat4f &projection)
{
    assert ( shader != NULL  && m_pMesh != NULL);
    m_pShader = shader;
    m_mProjViewModelMatrix = projection * view * m_mTransformationMatrix;
    
    if(m_pShader->matprojviewmodel != -1 )
        glUniformMatrix4fv(m_pShader->matprojviewmodel, 1, GL_FALSE, &m_mProjViewModelMatrix[0]);
    
    //recursive
    RenderNode(m_pAnimator->mScene->mRootNode);
}

