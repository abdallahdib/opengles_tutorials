/*
 *  WaterEntity.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "WaterEntity.h"
#include "CameraEntity.h"

CWaterEntity::CWaterEntity(float32 waterHeight, CTexture* normaTexture, const vec2f& dimension)
: m_fWaterHeight(waterHeight), m_pOffscreenRT(NULL), m_iScreenHeight(480), m_iScreenWidth(320), m_pNormalTexture(normaTexture)
{
    m_pOffscreenRT = new COffscreenRenderTarget();
    m_pOffscreenRT->Initialize(kCONTEXT_COLOR | kCONTEXT_DEPTH,  256);
    m_mMirror.identity();
    m_mMirror[5] = -1.f;
    m_mTransformationMatrix = mat4f::createTranslation(0, 0, waterHeight);
    CreatePlane(dimension);
}

CWaterEntity::~CWaterEntity()
{
    SAFE_DELETE(m_pOffscreenRT);
    SAFE_DELETE(m_pMeshBuffer);
}
void CWaterEntity::CreatePlane(const vec2f& dimension)
{
    m_pMeshBuffer = new CMeshBuffer();
    int32 numVertices = 4;
    
    CMeshGroup* group = new CMeshGroup();
    std::vector<CGpuVertex> &vert = group->GetVertices();
    std::vector<ushort16> &ind = group->GetIndices();
    group->SetMaterialIndex(0);
    
    
    CGpuVertex vertex;
    vertex.pos = vec3f(0.5f,  0.5f, 0.0f);
    vertex.normal = vec3f(0.0f, 0.0f, 1.0f);
    vertex.texCoord = vec2f(1.f, 1.f);
    vert.push_back(vertex);
    
    
    vertex.pos = vec3f(-0.5f, -0.5f, 0.0f);
    vertex.normal = vec3f(0.0f, 0.0f, 1.0f);
    vertex.texCoord = vec2f(0.0f, 0.0f);
    vert.push_back(vertex);
    
    
    vertex.pos = vec3f(-0.5f,  0.5f, 0.0f);
    vertex.normal = vec3f(0.0f, 0.0f, 1.0f);
    vertex.texCoord = vec2f(0.0f, 1.f);
    vert.push_back(vertex);
    
    
    vertex.pos = vec3f(0.5f, -0.5f, 0.0f);
    vertex.normal = vec3f(0.0f, 0.0f, 1.0f);
    vertex.texCoord = vec2f(1.f, 0.0f);
    vert.push_back(vertex);
    
    for (int32 i= 0; i< numVertices; i++)
    {
        vert[i].pos.x *=  dimension.x;
        vert[i].pos.y *=  dimension.y;
        
    }
    
    CBoundingBox box;
    for (int32 i= 0; i< numVertices; i++)
        box.Add(vert[i].pos);
    
    group->SetBoundingBox(box);
    ind.push_back((ushort16)0);
    ind.push_back((ushort16)2);
    ind.push_back((ushort16)1);
    
    ind.push_back((ushort16)0);
    ind.push_back((ushort16)1);
    ind.push_back((ushort16)3);
    
    group->AllocateOnGpuMemory(kGL_BUFFER_USAGE_HINT_STATIC);
    m_pMeshBuffer->AddGroup(group);
    
}
void CWaterEntity::Update(float32 dt)
{
}

void CWaterEntity::PrepareReflectionPass(CShader* shader, CCameraEntity* camera)
{
    //enable fbo for offscreen rendering
    m_pOffscreenRT->Enable();
    
    //todo: replace the discard from the pixel shader by a clipping plane.
    shader->SetUniform1i("enableDiscardScene",1) ;
    shader->SetUniform1f("waterHeight", m_fWaterHeight);
    
    //clear context
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    //get the camera position,target
    vec3f camPos = camera->GetPosition();
    vec3f camTarget = camPos + camera->GetForwardVector();
    
    //invert camera based on the current water position    
    //if camera is on the top of the water invert the camera position.
    //here we dont deal with the case where the camera is under the water surface, in this situation we have to render an addition refraction texture. 
    if(camPos.z >= m_fWaterHeight)
    {
        //the corresponding mirrored position 
        camPos.z = 2.0f * m_fWaterHeight - camPos.z;
        camTarget.z = 2.0f * m_fWaterHeight - camTarget.z;

        vec3f forwardVector = camTarget - camPos;
        vec3f sideVector = camera->GetRigthVector();
        vec3f reflectionCamUp = sideVector.crossProduct(forwardVector);
        
        //setup the new mirrored camera position
        mat4f m;
        m = mat4f::createLookAt(camPos, camTarget, reflectionCamUp);
        m_mLookAt =  m_mMirror * m;
    }
}

void CWaterEntity::EndReflectionPass(CShader* shader)
{
    shader->SetUniform1i("enableDiscardScene",0) ;
    
    //disable offscreen rendering context
    m_pOffscreenRT->Disable();

    //get the reflection texture ( this can be done once)
    m_pReflectionTexture = m_pOffscreenRT->GetColorBuffer();
}

void CWaterEntity::Render(CShader* shader, const mat4f &view, const mat4f &projection)
{
    assert(m_pNormalTexture != NULL && shader != NULL);
    
    //quick and dirty time 
    static float32 t = 0.f;
    t += 0.001f;
    
    //send modelviewProj matrix to the shader if needed
    if(shader->matprojviewmodel != -1 )
    {
        mat4f mat = projection * view * m_mTransformationMatrix;
        glUniformMatrix4fv(shader->matprojviewmodel, 1, GL_FALSE, &mat[0]);
    }
    
    //send necessary params to the shader ( time, normal texure)
    m_pReflectionTexture->ActivateAndBind(GL_TEXTURE0);
    shader->SetUniform1i("texture0", 0, shader->texture0);
    m_pNormalTexture->ActivateAndBind(GL_TEXTURE1);
    shader->SetUniform1i("normal", 1);
    shader->SetUniform1f("time",t) ;
    
    //shader needs to know the screen dimension
    shader->SetUniform1f("screenWidth",  1.f / (float32)m_iScreenWidth) ;
    shader->SetUniform1f("screenHeight", 1.f / (float32)m_iScreenHeight) ;
    
    //the water surface is composed of a simple quad. map the group and render it  
    CMeshGroup* grp = m_pMeshBuffer->GroupAtIndex(0);
    grp->MapToGPU(0);
    glDrawElements(grp->GetDrawingMode(), grp->GetIndices().size(), GL_UNSIGNED_SHORT, 0);
}

void CWaterEntity::SetScreenWidthAndHeight(int32 width, int32 height)
{
    m_iScreenWidth = width;
    m_iScreenHeight = height;
}
