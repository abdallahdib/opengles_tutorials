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
: m_pCamera(NULL), m_pMesh(NULL), m_pWater(NULL), m_pSkybox(NULL), m_pHouse(NULL)
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
    SAFE_DELETE(m_pWater);
    SAFE_DELETE(m_pSkybox);
    SAFE_DELETE(m_pHouse);
}
bool Tutorial::LoadShaders()
{
    //load shader
    m_pShaderMesh = crm.LoadShader("Terrain");
    if(!m_pShaderMesh)
        return false;
    
    //sky vox shader
    m_pShaderSkybox = crm.LoadShader("SkyBox");
    if(!m_pShaderSkybox)
        return false;
    
    //water surface shader
    m_pShaderWaterSurface = crm.LoadShader("WaterSurface");
    if(!m_pShaderWaterSurface)
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
    
    m_pMesh->GetMeshBuffer().MaterialAtindex(0)->detailTexture = crm.LoadTexture2D("detail.jpg");
    m_pMesh->GetMeshBuffer().MaterialAtindex(0)->detailFactor = 6.f;
    
    //bumped mesh
    IMesh* bumpedAssimpMesh = IMesh::LoadMeshFromFile("house_obj.obj");
    if(bumpedAssimpMesh == NULL)
        return false;
    
    m_pHouse = new CMeshEntity(bumpedAssimpMesh);
    m_pHouse->GetMeshBuffer().MaterialAtindex(1)->detailTexture = crm.LoadTexture2D("detail.jpg");//add detail to house
    m_pHouse->GetMeshBuffer().MaterialAtindex(1)->detailFactor = 6.f;
    
    //water
    CTexture* normal = crm.LoadTexture2D("normal.jpg");
    if(normal == NULL)
        return false;
    
    m_pWater = new CWaterEntity(-30.f, normal, vec2f(5000.f, 5000.f));
    
    //skybox
    m_pSkybox = new CSkyBoxEntity(1000.f);
    CTextureCubeMap *textureCubeMap = crm.LoadTextureCube("sky.jpg");
    
    if(textureCubeMap == NULL)
        return false;
    
    m_pSkybox->Create(textureCubeMap );
    
    //m_pCamera
    m_pCamera = new CCameraEntity(vec3f(-5.72926, -170.729, -15.5698), vec3f(-4.99851, -170.047, -15.6047), 40.f);
    
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
    //enable depth test and face culling
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    //setup alpha blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
    //draw all objects of the scene
    m_pShaderSkybox->Begin();
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    m_pSkybox->Render(m_pShaderSkybox, view, projection);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    m_pShaderSkybox->End();
    
    m_pShaderMesh->Begin();
    m_pMesh->Render(m_pShaderMesh, view, projection);
    mat4f &modelHouse = m_pHouse->GetTransfromationMatrix();
    mat4f s;
    s.identity();
    //scale the model ( this can be done within a modeling package ) scaling can affect light calculation
    s[0] *= 0.03f;
    s[5] *= 0.03f;
    s[10] *= 0.03f;
    modelHouse = s;
    //position the model on the terrain
    modelHouse[12] = 117.f;
    modelHouse[13] = -113.4579f;
    modelHouse[14] = -28.0493;
    m_pHouse->Render(m_pShaderMesh, view, projection);
    m_pShaderMesh->End();
    
    
}
void Tutorial::Frame()
{
    //clear depth/color buffer bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set Main shader
    m_pShaderMesh->Begin();
    
    //1- FIRST PASS: from mirrored camera position ( this will capture the reflected scene)
    //cull front faces
    glCullFace(GL_FRONT);
    //this will enable the framebuffer object for offscreen rendering where the mirrored scene in rendered to the attached texture
    m_pWater->PrepareReflectionPass(m_pShaderMesh, m_pCamera);
    RenderFromPosition(m_pWater->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
    //this will end the reflection pass and switch back to default frame buffer
    m_pWater->EndReflectionPass(m_pShaderMesh);
    glCullFace(GL_BACK);
    
    //2- SECOND PASS: draw the scene from camera position
    RenderFromPosition(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
    
    //3- Draw water surface using the mirrored texture rendered in the first pass
    m_pShaderWaterSurface->Begin();
    m_pWater->Render(m_pShaderWaterSurface, m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
    m_pShaderWaterSurface->End();

}
void Tutorial::SetWindowSize(float32 width, float32 height)
{
    if(m_pWater != NULL)
        m_pWater->SetScreenWidthAndHeight(width, height);
}