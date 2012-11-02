/* Tutorial.h
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

#ifndef vvision_Tutorial_h
#define vvision_Tutorial_h

#include "vvision.h"
#include "CameraEntity.h"
#include "MeshEntity.h"

struct CControlPad
{
    /** this is set to true if key up is pressed*/
    bool keyUp;
    
    /** true if keyDown pressed*/
    bool keyDown;
    
    /** left*/
    bool keyLeft;
    
    /** right*/
    bool keyRight;
    
    /** key move forward is pressed*/
    bool keyMoveUp;
    
    /** key move backward is pressed*/
    bool keyMoveDown;
    
    /** button switch*/
    bool keySwitch;
};

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
    
    /** */
    void UpdateControls(const CControlPad& pad);
    
private:
    
    /** cleanup*/
    void Cleanup();
    
    /** load shaders*/
    bool LoadShaders();
    
    /** load entities*/
    bool LoadEntities();
    
    /**render the scene from different position*/
    void RenderFromPosition(const mat4f& view, const mat4f& projection);
    
    /** this will map shader attribs for a given mesh.
     * it calculates the light position and camera position in the model space and map it to the shader
     */
    void MapShaderLightCamPosForMesh(CMeshEntity* mesh, const vec4f& lightPos, const vec4f& camPos );
    
    /** shaders*/
    CShader* m_pShaderBump;
    CShader* m_pShaderNoBump;
    CShader* m_pActiveShader;
    
    /** m_pCamera entity*/
    CCameraEntity* m_pCamera;
    
    /** bumped model*/
    CMeshEntity* m_pBumpModel;
    CMeshEntity* m_pBumpModel2;
    
    /** time*/
    float32 time;
};

#endif
