/* Tutorial.h
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

#ifndef vvision_Tutorial_h
#define vvision_Tutorial_h

#include "vvision.h"
#include "CameraEntity.h"
#include "AnimatedMeshEntity.h"


class Tutorial
{
public:
    
    /** constructor*/
    Tutorial();
    
    /** destructor*/
    ~Tutorial();
    
    /** deploy tutorial content*/
    bool Deploy();
    
    /**display frame*/
    void Frame();
    
    /** set the screen size screen width and height ( this can be retreived from glGetViewPort.)
     this value is calculated once when the main frame buffer is created, at this time we can upate the window size in order to avoid calling glGetViewPort multiple times.
     */
    void SetWindowSize(float32 width, float32 height);
private:
    
    /** cleanup*/
    void Cleanup();
    
    /** load shaders*/
    bool LoadShaders();
    
    /** load entities*/
    bool LoadEntities();
    
    /**render the scene from different position*/
    void RenderFromPosition(const mat4f& view, const mat4f& projection);
    
    /** shaders*/
    CShader *m_pShaderAnimatedMesh;
    
    /** m_pCamera entity*/
    CCameraEntity* m_pCamera;
    
    /** animated mesh entity*/
    CAnimatedMeshEntity* m_pAnimatedMesh;
    
    /** time*/
    float32 time;
    

};

#endif
