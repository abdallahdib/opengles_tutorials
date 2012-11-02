/*
 *  CameraEntity.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */
#include "CameraEntity.h"

CCameraEntity::CCameraEntity(const vec3f& position, const vec3f& target, float32 fov, const vec3f& up, float32 zNear, float32 zFar, float32 aspect)	
: 
m_vPosition(position)
{
    m_mProjection = mat4f::createPerspective(fov, aspect, zNear, zFar);
    m_mView = mat4f::createLookAt(m_vPosition, target, up);
    m_qQuatuernion = Quatf::frommat(m_mView);
}

CCameraEntity::~CCameraEntity()
{
}

void CCameraEntity::Update(float32 dt)
{
}
const Quatf& CCameraEntity::GetQuaternionRepresentation()
{
    return m_qQuatuernion;
}
void CCameraEntity::SetQuaternion(Quatf new_quat)
{
    //set the new quaternion
    m_qQuatuernion = new_quat;
    
    //extract view matrix from quaternion
    m_mView = m_qQuatuernion.transform();

    //update world view matrix
    m_mWorldView = m_mView.inverse();
    m_mWorldView[12] = m_vPosition.x;
    m_mWorldView[13] = m_vPosition.y;
    m_mWorldView[14] = m_vPosition.z;
    m_mView = m_mWorldView.inverse();
}
const mat4f& CCameraEntity::GetViewMatrix()
{
    //get the view matrix of the camera
    m_mView = m_qQuatuernion.transform();
    m_mWorldView = m_mView.inverse();
    m_mWorldView[12] = m_vPosition.x;
    m_mWorldView[13] = m_vPosition.y;
    m_mWorldView[14] = m_vPosition.z;
    m_mView = m_mWorldView.inverse();
   
    return m_mView;
}
void CCameraEntity::SetPosition(const vec3f& position)
{
    m_vPosition = position;
}

void CCameraEntity::SetProjectionMatrix(const mat4f& projection)
{
    m_mProjection = projection;
}

void CCameraEntity::Render(CShader* shader)
{
    if(shader->matview != -1 )
        glUniformMatrix4fv(shader->matview, 1, GL_FALSE, &GetViewMatrix()[0]);
    
    if(shader->matproj != -1)
       glUniformMatrix4fv(shader->matview, 1, GL_FALSE, &m_mProjection[0]);
}
vec3f CCameraEntity::GetPosition()
{
    m_mWorldView = m_mView.inverse();
    return vec3f(m_mWorldView[12], m_mWorldView[13], m_mWorldView[14]);
}
vec3f CCameraEntity::GetForwardVector()
{
     m_mWorldView = m_mView.inverse();
    return vec3f(-m_mWorldView[8], -m_mWorldView[9], -m_mWorldView[10]);
}
vec3f CCameraEntity::GetUpVector()
{
    m_mWorldView = m_mView.inverse();
    return vec3f(m_mWorldView[4], m_mWorldView[5], m_mWorldView[6]);
}
vec3f CCameraEntity::GetRigthVector()
{
    m_mWorldView = m_mView.inverse();
    return vec3f(m_mWorldView[0], m_mWorldView[1], m_mWorldView[2]);
}
