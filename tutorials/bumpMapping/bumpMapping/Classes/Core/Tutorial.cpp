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
: m_pCamera(NULL), m_pBumpModel(NULL), time(0.f)
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
    SAFE_DELETE(m_pBumpModel);
}
bool Tutorial::LoadShaders()
{
    //bmp shader
    m_pShaderBump = crm.LoadShader("Bump");
    if(!m_pShaderBump)
        return false;
    
    return true;
}

bool Tutorial::LoadEntities()
{
    
    //bumped mesh
    IMesh* bumpedAssimpMesh = IMesh::LoadMeshFromFile("Crate_Fragile.obj");
    if(bumpedAssimpMesh == NULL)
        return false;
    
    m_pBumpModel = new CMeshEntity(bumpedAssimpMesh);
    m_pBumpModel->GetMeshBuffer().MaterialAtindex(1)->bumpTexture = crm.LoadTexture2D("fragile_normal.jpg");//our material is located at index 1
    
    //m_pCamera
    m_pCamera = new CCameraEntity(vec3f(-47.8f, 2.3f, 5.1f), vec3f(-46.8f, 2.7f, 4.9f), 70.f);
    
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    return true;
}
void Tutorial::RenderFromPosition(const mat4f& view, const mat4f& projection)
{
    m_pShaderBump->Begin();
    mat4f &modelBumpMeshMat =  m_pBumpModel->GetTransfromationMatrix();
    mat4f trans = mat4f::createTranslation(3, 0, -3);
    modelBumpMeshMat = trans;
    
    //animate light on a circular trajectory
    static float y = 0.f;
    static float x = 0.f;
    x = -80.f * sinf(time);
    y = -80.f * cosf(time);
    //transform light to model space, so light calculation can be peformed in model space within shader
    //to do so we multiply the light position by the inverse of the model matrix of the model.
    vec4f lightPos = modelBumpMeshMat.inverse() * vec4f(x, y, 0, 1.f);//dont forget that inverse of rotation matrix is the transpose
    m_pShaderBump->SetUniform3fv("LightPosModel", 1, &lightPos.x);
    m_pBumpModel->Render(m_pShaderBump, view, projection);
    m_pShaderBump->End();
}
void Tutorial::Frame()
{
    //clear depth/color buffer bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderFromPosition(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
    
    //replace with the real ellapsed time
    time += 0.01667f;  // 1/ 60.f
}