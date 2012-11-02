/*
 *  Tutorial.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "Tutorial.h"

//#define RENDER_SKYBOX

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
    m_pShaderMesh = crm.LoadShader("cubeMapping");
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
    IMesh* assimpMesh = IMesh::LoadMeshFromFile(("teapot1024.3DS"));//( knot2880_nomap.3DS) models from ozone3D

    if(assimpMesh == NULL)
        return false;
    
    m_pMesh = new CMeshEntity(assimpMesh);
        
    //skybox
    m_pSkybox = new CSkyBoxEntity(1000.f);
    CTextureCubeMap *textureCubeMap = crm.LoadTextureCube("cm.jpg");
    
    if(textureCubeMap == NULL)
        return false;
    
    m_pSkybox->Create(textureCubeMap );
    
    //
    m_pMesh->GetMeshBuffer().MaterialAtindex(0)->cubeMapTexture = textureCubeMap;
    
    //m_pCamera
    m_pCamera = new CCameraEntity(vec3f(3.7283f, -51.8147f, 192.79f), vec3f(3.71504f, -51.442f, 191.863f), 70.f, vec3f(0, 0.f, 1.f), 1.f, 10000.f, 1.5f);
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
    //activate shader
    m_pShaderMesh->Begin();
    
    //get the model matrix
    mat4f &model = m_pMesh->GetTransfromationMatrix();
    
    /*to find the camera position in the model space:
     1- camera or eye position is located at the v0( 0, 0, 0 ), we multiply v0 by the inverse of the view matrix (V) then by the inverse of model matrix ( M). camPos = inv(M) * inv(V) * vec4(0, 0, 0, 1); 
     */
    
    //try
    //vec4f camera_pos = ( model.inverse() )* vec4f(m_pCamera->GetPosition());
    //or
    vec4f camera_pos = ( m_pCamera->GetViewMatrix() * model ).inverse() * vec4f(0, 0, 0, 1.f);
    
    //extract the rotation matrix
    mat3f model_mat3 = mat3f(model);

    //send uniforms to shader
    m_pShaderMesh->SetUniform4fv("camPosModel", 1, &camera_pos.x);
    m_pShaderMesh->SetUniformMatrix3x3fv("matModelToWorld", 1, GL_FALSE, &model_mat3[0]);
    
    //render the mesh 
    m_pMesh->Render(m_pShaderMesh, view, projection);
    
    //disable shader
    m_pShaderMesh->End();
    
    //set this flag to draw the skybox
#ifdef RENDER_SKYBOX
    //draw all objects of the scene
    m_pShaderSkybox->Begin();
    glDepthMask(GL_FALSE);//this can be added as a material property so a scene manager can set th GL state accordingly
    glDisable(GL_CULL_FACE); // this is a material property (CMaterial has an attribute named twoSided)
    m_pSkybox->Render(m_pShaderSkybox, view, projection);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    m_pShaderSkybox->End();
#endif
    
    //rotate model
    static float angle = 0.f;
    model = mat4f::createRotationAroundAxis(angle, 0, angle);
    angle += 1.f;
}
void Tutorial::Frame()
{
    //clear depth/color buffer bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //render the scene from the camera position
    RenderFromPosition(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
    //cerr<<m_pCamera->GetPosition()<<" ->"<<m_pCamera->GetPosition() + m_pCamera->GetForwardVector()<<endl;
}
