/* WaterEntity.h
 *
 * Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef VVISION_Water_Entity_h
#define VVISION_Water_Entity_h

#include "vvision.h"

class CCameraEntity;

class CWaterEntity
{
public:
    
    /** constructor
     @param waterHeight the height level of the water ( this is used to cull objects under water surface), this can be replaced with a complete plane equation, in this tutorial we suppose that the water is at z = waterHeight
     @param normaTexture, a normal texture to perfom the water effect 
     @param dimension, the surfrace of the water 
     */
    CWaterEntity(float32 waterHeight, CTexture* normaTexture, const vec2f& dimension);
    
    /** destructor*/
    ~CWaterEntity();
    
    /** update mesh*/
    virtual void Update(float32 dt);
    
    /** prepare for the reflection pass  ( this should be balanced with EndReflectionPass)*/
    void PrepareReflectionPass(CShader* shader, CCameraEntity* camera);
    
    /** end of the reflection pass ( this should be balanced with PrepareReflectionPass)*/
    void EndReflectionPass(CShader* shader);
    
    /** render water surface*/
    virtual void Render(CShader* shader, const mat4f &view, const mat4f &projection);
    
    /** water entity needs to know the screen width and height to render correctly*/
    void SetScreenWidthAndHeight(int32 width, int32 height);
    
    const mat4f& GetViewMatrix() {return m_mLookAt;}
private:
    
    /** plane vertices that rerpresent the water surface*/
    void CreatePlane(const vec2f& dimension);
    
    /** water surface height */
    float32 m_fWaterHeight;
    
    /**offscreen render target*/
    COffscreenRenderTarget* m_pOffscreenRT;
    
    /** transformation matrix*/
    mat4f m_mTransformationMatrix;
    
    /** mirror matrix*/
    mat4f m_mMirror;
    mat4f m_mLookAt;
    
    /** screen width and height*/
    int32 m_iScreenWidth;
    int32 m_iScreenHeight;
    
    /**normal texture*/
    CTexture* m_pNormalTexture;
    
    /** mesh buffer*/
    CMeshBuffer* m_pMeshBuffer;
    
    /** reflection texture*/
    CTexture* m_pReflectionTexture;
    
};

#endif
