/* CameraEntity.h
 *
 * Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef VVISION_CameraEntity_h
#define VVISION_CameraEntity_h


#include "vvision.h"


class CCameraEntity
{
public:
    
    /** create a perspective camera
     @param position camera position
     @param target camera target point
     @param fov field of view
     @param up the up vector
     @param zNear near clipping plane of the camera
     @param zFar far clipping plane of the camera
     @param aspect ration of the camera ( width / height)
     */
    CCameraEntity(const vec3f& position, const vec3f& target, float32 fov, const vec3f& up = vec3f(0, 0.f, 1.f), float32 zNear = 0.1f, float32 zFar = 10000.0f, float32 aspect = 1.5f);
    
    /** destructor*/
    ~CCameraEntity();
    
    /** update camera*/
    virtual void Update(float32 dt);
    
    /** render ( this should register the view/projection matrix of the camera within the active shader)*/
    virtual void Render(CShader* shader);
    
    /** return the projection matrix of the camera*/
    const mat4f& GetProjectionMatrix() {return m_mProjection;}
    
    /** return the view matrix  ( this is extracted from the quaternion*/
    const mat4f& GetViewMatrix();
    
    /** return the quaternion representation of the camera*/
    const Quatf& GetQuaternionRepresentation();
    
    /** get camera position*/
    vec3f GetPosition();
    
    /** forward vector*/
    vec3f GetForwardVector();
    
    /** the up vector*/
    vec3f GetUpVector();
    
    /** get right vector*/
    vec3f GetRigthVector();
    
    /** set a new projection matrix for the camera ( ortho, frustum, todo: add helper for creating ortho frustum)*/
    void SetProjectionMatrix(const mat4f& projection);
    
    /** set camera position*/
    void SetPosition(const vec3f& position);
    
    /** set new quaternion for the camera*/
    void SetQuaternion(Quatf new_quat);
    
private:

    /** quaternion representation*/
    Quatf m_qQuatuernion;
    
    /** view matrix*/
    mat4f m_mView;
    
    /** world view matrix*/
    mat4f m_mWorldView;
    
    /** projection*/
    mat4f m_mProjection;
    
    /** camera position*/
    vec3f m_vPosition;
    
};

#endif
