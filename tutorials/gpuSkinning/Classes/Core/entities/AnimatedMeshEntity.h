/* AnimatedMeshEntity.h
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


#ifndef VVISION_AnimatedMeshEntity_h
#define VVISION_AnimatedMeshEntity_h

#include "vvision.h"
#include "AssimpMesh.h"

class CAnimatedMeshEntity
{
public:
    
    /** constructor*/
    CAnimatedMeshEntity(CAssimpMesh* mesh);
    
    /** destructor*/
    ~CAnimatedMeshEntity();
    
    /** update mesh*/
    virtual void Update(float32 dt);
    
    /** render*/
    virtual void Render(CShader* shader, const mat4f &view, const mat4f &projection);
    
    /** returns a reference to the transformation matrix of the model*/
    inline mat4f& GetTransfromationMatrix() {return m_mTransformationMatrix;}
    
private:
    /** render mesh recursivly */
    void RenderNode(aiNode* piNode);
    
    /**the mesh*/
    CAssimpMesh* m_pMesh;
    
    /** animator ( calculate bone matrices) */
    AssimpSceneAnimator* m_pAnimator;
    
    /** global transformation matrix of the model*/
    mat4f m_mTransformationMatrix;
    
    /** shader for gpu skinning*/
    CShader* m_pShader;
    
    /** cache projeciton * view * matrix */
    mat4f m_mProjViewModelMatrix;
};

#endif
