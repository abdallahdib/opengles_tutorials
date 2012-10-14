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
: m_pCamera(NULL), m_pMesh(NULL), m_pShaderMesh(NULL)
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
    SAFE_DELETE(m_pMesh);
}
bool Tutorial::LoadShaders()
{
    //load shader
    m_pShaderMesh = crm.LoadShader("nolighting");
    return (m_pShaderMesh != NULL);
}

bool Tutorial::LoadEntities()
{
    //static mesh
    IMesh* assimpMesh = IMesh::LoadMeshFromFile(("house_obj.obj"));

    if(assimpMesh == NULL)
        return false;
    
    m_pMesh = new CMeshEntity(assimpMesh);
    
    //m_pCamera
    m_pCamera = new CCameraEntity(vec3f(-479.7f, -935.5f, 492.7f), vec3f(-391.6f, -737.5f, 418.7f), 70.f);
    return true;
}

bool Tutorial::Deploy()
{
    if(!LoadShaders() || !LoadEntities())
    {
        Cleanup();
        return false;
    }
    
    //setup opengl 
    glClearColor(0.2, 0.2, 0.2, 1.f);
    //enable depth test an back face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    return true;
}
void Tutorial::RenderFromPosition(const mat4f& view, const mat4f& projection)
{
    //activate shader
    m_pShaderMesh->Begin();
    //render model
    m_pMesh->Render(m_pShaderMesh, view, projection);
    //disable shader
    m_pShaderMesh->End();
    
    //roate model arround vertical axis
    static float angle = 0.f;
    mat4f & model = m_pMesh->GetTransfromationMatrix();
    model = mat4f::createRotationAroundAxis(0, 0, angle);
    angle += 0.5f;
}
void Tutorial::Frame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderFromPosition(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
}