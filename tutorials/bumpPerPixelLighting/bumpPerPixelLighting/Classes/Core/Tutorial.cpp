/*
 *  Tutorial.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "Tutorial.h"

#define DIFFUSE_TEXTURE1 "rocks_diffuse.jpg"
#define NORMAL_TEXTURE1 "rocks_normal.jpg"
//try this

//#define DIFFUSE_TEXTURE1 "fragile_diffuse.jpg"
//#define NORMAL_TEXTURE1 "fragile_normal.jpg"

//or this
//#define DIFFUSE_TEXTURE1 "stones_diffuse.jpg"
//#define NORMAL_TEXTURE1 "stones_normal.jpg"

#define DIFFUSE_TEXTURE2 "color_map.jpg"
#define NORMAL_TEXTURE2 "normal_map2.jpg"


#define LIGHT_AMBIENT   vec4f(0.2f, 0.2f, 0.2f, 1.f)
#define LIGHT_DIFFUSE   vec4f(1.0f, 1.0f, 1.0f, 1.f);
#define LIGHT_SPECULAR  vec4f(1.f, 1.f, 0.9f, 1.f);


//cache resource manager
CCacheResourceManager& crm = CCacheResourceManager::Instance();

Tutorial::Tutorial()
: m_pCamera(NULL), m_pBumpModel(NULL), m_pBumpModel2(NULL), time(0.f)
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
    SAFE_DELETE(m_pBumpModel2);
}
bool Tutorial::LoadShaders()
{
    //bmp shader
    m_pShaderBump = crm.LoadShader("Bump");
    if(!m_pShaderBump)
        return false;
    
    //no bump shader
    m_pShaderNoBump = crm.LoadShader("NoBump");
    if(!m_pShaderNoBump)
        return false;
    
    //default selected shader is the bump
    m_pActiveShader = m_pShaderBump;
    return true;
}

bool Tutorial::LoadEntities()
{
    
    //bumped mesh
    IMesh* bumpedAssimpMesh = IMesh::LoadMeshFromFile("box.obj");
    if(bumpedAssimpMesh == NULL)
        return false;
    
    m_pBumpModel = new CMeshEntity(bumpedAssimpMesh);
    
    IMesh* bumpedAssimpMesh2 = IMesh::LoadMeshFromFile("sphere.obj");
    if(bumpedAssimpMesh2 == NULL)
        return false;
    
    m_pBumpModel2 = new CMeshEntity(bumpedAssimpMesh2);
    
    //
    int32 matIndex = 1;
    //material properties for first model
    m_pBumpModel->GetMeshBuffer().MaterialAtindex(matIndex)->diffuseTexture = crm.LoadTexture2D(DIFFUSE_TEXTURE1);//set diffuse texture
    m_pBumpModel->GetMeshBuffer().MaterialAtindex(matIndex)->bumpTexture = crm.LoadTexture2D(NORMAL_TEXTURE1);// set bump texture
    m_pBumpModel->GetMeshBuffer().MaterialAtindex(matIndex)->ambient = vec4f(0.2, 0.2, 0.2, 1.0);//set ambient
    m_pBumpModel->GetMeshBuffer().MaterialAtindex(matIndex)->diffuse = vec4f(0.8, 0.8, 0.8, 1.0);//set diffuse
    m_pBumpModel->GetMeshBuffer().MaterialAtindex(matIndex)->specular = vec4f(1.0, 1.0, 1.0, 1.0);//set specular
    m_pBumpModel->GetMeshBuffer().MaterialAtindex(matIndex)->shininess = 60.0;//set shininess
    
    //material properties for second mesh
    m_pBumpModel2->GetMeshBuffer().MaterialAtindex(matIndex)->diffuseTexture = crm.LoadTexture2D(DIFFUSE_TEXTURE2);//set diffuse texture
    m_pBumpModel2->GetMeshBuffer().MaterialAtindex(matIndex)->bumpTexture = crm.LoadTexture2D(NORMAL_TEXTURE2);// set bump texture
    m_pBumpModel2->GetMeshBuffer().MaterialAtindex(matIndex)->ambient = vec4f(0.2, 0.2, 0.2, 1.0);//set ambient
    m_pBumpModel2->GetMeshBuffer().MaterialAtindex(matIndex)->diffuse = vec4f(0.8, 0.8, 0.8, 1.0);//set diffuse
    m_pBumpModel2->GetMeshBuffer().MaterialAtindex(matIndex)->specular = vec4f(1.0, 1.0, 1.0, 1.0);//set specular
    m_pBumpModel2->GetMeshBuffer().MaterialAtindex(matIndex)->shininess = 10.0;//set shininess
    

    //m_pCamera
    m_pCamera = new CCameraEntity(vec3f(-2.20533, 6.71143, -4.58115), vec3f(-1.74897, 7.35103, -5.19973), 70.f);
    
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
    return true;
}
void Tutorial::MapShaderLightCamPosForMesh(CMeshEntity* mesh, const vec4f& lightPos, const vec4f& camPos)
{
    //get the model matrix
    mat4f &modelBumpMeshMat =  mesh->GetTransfromationMatrix();
    mat4f modelInv = modelBumpMeshMat.inverse();
    
    //light position in the model space
    vec4f lightPosition = modelInv * lightPos;
    
    //camera position in the model space
    vec4f cameraPos= modelInv * camPos;
    
    //send uniforms to shader
    m_pActiveShader->SetUniform3fv("lightPosModel", 1, &lightPosition.x);
    m_pActiveShader->SetUniform3fv("camPosModel", 1, &cameraPos.x);
    
}
void Tutorial::RenderFromPosition(const mat4f& view, const mat4f& projection)
{
    m_pActiveShader->Begin();

    //light position
    vec3f camPos = m_pCamera->GetPosition();
    vec4f lightPos = vec4f(camPos.x, camPos.y, camPos.z, 1.f);
    
    //light properties
    vec4f lightAmbient = LIGHT_AMBIENT;
    vec4f lightDiffuse = LIGHT_DIFFUSE;
    vec4f lightSpecular = LIGHT_SPECULAR;
    m_pActiveShader->SetUniform4fv("lightColorAmbient", 1, &lightAmbient.x);
    m_pActiveShader->SetUniform4fv("lightColorDiffuse", 1, &lightDiffuse.x);
    m_pActiveShader->SetUniform4fv("lightColorSpecular", 1, &lightSpecular.x);
    
    
    m_pActiveShader->SetUniform1f("repeatTexture", 1.2f);
    MapShaderLightCamPosForMesh(m_pBumpModel, lightPos, camPos);
    m_pBumpModel->Render(m_pActiveShader, view, projection);
    
    m_pActiveShader->SetUniform1f("repeatTexture", 4.f);
    MapShaderLightCamPosForMesh(m_pBumpModel, lightPos, camPos);
    m_pBumpModel2->Render(m_pActiveShader, view, projection);
   
    m_pActiveShader->End();
    
    //cerr<<m_pCamera->GetPosition()<<" "<<m_pCamera->GetForwardVector() + m_pCamera->GetPosition()<<endl;
    
    
}
void Tutorial::Frame()
{
    //clear depth/color buffer bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderFromPosition(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
    
    //replace with the real ellapsed time
    time += 0.01667f;  // 1/ 60.f
}
void Tutorial::UpdateControls(const CControlPad& pad)
{
    const float step = 0.6f;
    
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
        pos += right * step;
        m_pCamera->SetPosition(pos);
        
    }
    
    else if(pad.keyMoveDown)
    {
        vec3f pos = m_pCamera->GetPosition();
        vec3f right = m_pCamera->GetForwardVector();
        pos -= right * step;
        m_pCamera->SetPosition(pos);
    }
    
    // we dont have to verify the current pad state each frame, an event dispatch mechanism should be used in real conditions.
    m_pActiveShader = (pad.keySwitch) ? m_pShaderBump: m_pShaderNoBump;
}