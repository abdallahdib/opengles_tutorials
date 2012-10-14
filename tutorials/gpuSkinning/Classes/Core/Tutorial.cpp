/*
 *  Tutorial.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "Tutorial.h"

//cache resource manager
CCacheResourceManager& crm = CCacheResourceManager::Instance();

Tutorial::Tutorial()
: m_pCamera(NULL), m_pAnimatedMesh(NULL), time(0.f)
{
}

Tutorial::~Tutorial()
{
    Cleanup();
}
void Tutorial::Cleanup()
{
    //this will delete all cached resources ( textures, shaders and meshes)
    crm.Destroy();
    
    SAFE_DELETE(m_pCamera);
    SAFE_DELETE(m_pAnimatedMesh);
}
bool Tutorial::LoadShaders()
{
    //load gpu skinning shader
    m_pShaderAnimatedMesh = crm.LoadShader("GpuSkinnig");
    return (m_pShaderAnimatedMesh != NULL);
}

bool Tutorial::LoadEntities()
{
    //animated mesh
    CAssimpMesh* animatedAssimpMesh = (CAssimpMesh*)IMesh::LoadMeshFromFile("dwarf.x");

    if(animatedAssimpMesh == NULL)
        return false;
    
    m_pAnimatedMesh = new CAnimatedMeshEntity(animatedAssimpMesh);
    
    //m_pCamera
    m_pCamera = new CCameraEntity(vec3f(0.f, -70.4f, 30.9f), vec3f(0.f, -69.4f, 31.f), 70.f);
    
    return true;
    
}

bool Tutorial::Deploy()
{
    if(!LoadShaders() || !LoadEntities())
    {
        Cleanup();
        return false;
    }
    
    glClearColor(0.2, 0.2, 0.2, 1.f);
    //enable depth test and back face culling
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    return true;
}
void Tutorial::RenderFromPosition(const mat4f& view, const mat4f& projection)
{
    m_pShaderAnimatedMesh->Begin();
    //update bones transformations matrices for the current time
    m_pAnimatedMesh->Update(time);
    m_pAnimatedMesh->Render(m_pShaderAnimatedMesh, view, projection);
    m_pShaderAnimatedMesh->End();
}
void Tutorial::Frame()
{
    // rotate model
    mat4f &model = m_pAnimatedMesh->GetTransfromationMatrix();
    model = mat4f::createRotationAroundAxis(-90, 0, 0);
    mat4f rot = mat4f::createRotationAroundAxis(0, time * 100.f, 0);
    model = model * rot;
    
    //clear depth/color buffer bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    RenderFromPosition(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
    
    //replace with the real ellapsed time
    time += 0.01667f;  // 1/ 60.f
 
}
void Tutorial::SetWindowSize(float32 width, float32 height)
{
}