/*
 *  Tutorial.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "Tutorial.h"

#define REPLACE_DIFFUSE_TEXTURE

//cache resource manager
CCacheResourceManager& crm = CCacheResourceManager::Instance();

Tutorial::Tutorial()
: m_pCamera(NULL), m_pMesh(NULL), m_pSkybox(NULL)
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
    SAFE_DELETE(m_pSkybox);
    
}
bool Tutorial::LoadShaders()
{
    //load shader
    m_pShaderMesh = crm.LoadShader("Terrain");
    if(!m_pShaderMesh)
        return false;
    
    //sky box shader
    m_pShaderSkybox = crm.LoadShader("SkyBox");
    if(!m_pShaderSkybox)
        return false;
    
    return true;
}

bool Tutorial::LoadEntities()
{
    //static mesh
    IMesh* assimpMesh = IMesh::LoadMeshFromFile(("terrain2.ms3d"));

    if(assimpMesh == NULL)
        return false;
    
    m_pMesh = new CMeshEntity(assimpMesh);
    
    //load detail texture and set the repeat factor for multi texture
    m_pMesh->GetMeshBuffer().MaterialAtindex(0)->detailTexture = crm.LoadTexture2D("detail.jpg");
    m_pMesh->GetMeshBuffer().MaterialAtindex(0)->detailFactor = 6.f;
    
    //this is here only to show you how u can remove/load resources from the cache
#ifdef REPLACE_DIFFUSE_TEXTURE
    
    //remove texture resource from cache by name
    crm.DeleteResource(kRESOURCE_TYPE_TEXTURE2D, "grass.jpg");
    
    //or u can remove the resource by memory adress
    //crm.DeleteResourceByMemoryAddress(kRESOURCE_TYPE_TEXTURE2D, m_pMesh->GetMeshBuffer().MaterialAtindex(0)->diffuseTexture);
    //m_pMesh->GetMeshBuffer().MaterialAtindex(0)->diffuseTexture = NULL;
    
    //load a new diffuse texture 
    m_pMesh->GetMeshBuffer().MaterialAtindex(0)->diffuseTexture = crm.LoadTexture2D("grassYellow.jpg");//replace diffuse texture
#endif
    
    //skybox
    m_pSkybox = new CSkyBoxEntity(1000.f);
    CTextureCubeMap *textureCubeMap = crm.LoadTextureCube("sky.jpg");
    
    if(textureCubeMap == NULL)
        return false;
    
    m_pSkybox->Create(textureCubeMap );
    
    //m_pCamera
    m_pCamera = new CCameraEntity(vec3f(0.f, -200.f, 25.f), vec3f(0.4f, -199.1f, 24.6f), 70.f);
    
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
    //enable depth test an back face culling
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    return true;
}
void Tutorial::UpdateControls(const CControlPad& pad)
{
    const float step = 2.f;
    
    //m_pCamera based quaternion
    const Quatf& q = m_pCamera->GetQuaternionRepresentation();//this will not call the copy construtor

    if(pad.keyUp)
    {
        Quatf rot = Quatf::fromAxisRot(vec3i(1, 0, 0), -step);
        Quatf qf = rot * q;
        m_pCamera->SetQuaternion(qf);
    }
    
    if(pad.keyDown)
    {
        Quatf rot = Quatf::fromAxisRot(vec3i(1, 0, 0), step);
        Quatf qf  = rot * q  ;
        m_pCamera->SetQuaternion(qf);
    }
    
    if(pad.keyLeft)
    {
        Quatf rotX = Quatf::fromAxisRot(vec3i(0, 0, 1), -step);
        Quatf qf  = q * rotX;
        m_pCamera->SetQuaternion(qf);
    }
    
    if(pad.keyRight)
    {
        Quatf rotX = Quatf::fromAxisRot(vec3i(0, 0, 1), step);
        Quatf qf  = q * rotX;
        m_pCamera->SetQuaternion(qf);
    }
    
    if(pad.keyMoveUp)
    {
        vec3f pos = m_pCamera->GetPosition();
        vec3f right = m_pCamera->GetForwardVector();
        pos += right * 2.f;
        m_pCamera->SetPosition(pos);
        
    }
    
    else if(pad.keyMoveDown)
    {
        vec3f pos = m_pCamera->GetPosition();
        vec3f right = m_pCamera->GetForwardVector();
        pos -= right * 2.f;
        m_pCamera->SetPosition(pos);
    }
    

}
void Tutorial::RenderFromPosition(const mat4f& view, const mat4f& projection)
{
    m_pShaderMesh->Begin();
    m_pMesh->Render(m_pShaderMesh, view, projection);
    m_pShaderMesh->End();
    
    //draw all objects of the scene
    m_pShaderSkybox->Begin();
    glDepthMask(GL_FALSE);//this can be added as a material property so a scene manager can set th GL state accordingly
    glDisable(GL_CULL_FACE); // this is a material property (CMaterial has an attribute named twoSided) 
    m_pSkybox->Render(m_pShaderSkybox, view, projection);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    m_pShaderSkybox->End();

}
void Tutorial::Frame()
{
    //clear depth/color buffer bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set Main shader
    m_pShaderMesh->Begin();
    RenderFromPosition(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
}
