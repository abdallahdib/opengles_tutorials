/* SkyBoxEntity.h
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

#ifndef SKY_BOX_ENTITY_INCLUDED
#define SKY_BOX_ENTITY_INCLUDED

#include "vvision.h"

class CSkyBoxEntity
{
    public :
    
    /** constructor
     * @param size the size of the sky box
     */
    CSkyBoxEntity( float32 size, const vec3f& position = vec3f());
    
    /** destructor*/
    ~CSkyBoxEntity();
    
    
    /** create skybox from a texture_base_name and size
     * @param texture_cubeMap the cube map texture used for the skybox
     * @return true if success
     */
    bool Create(CTextureCubeMap* texture_cubeMap);
    
    /** @inherited from Entity*/
    virtual void Render(CShader* shader, const mat4f &view, const mat4f &projection);
    

private:
    
    /** the mesh buffer*/
    CMeshBuffer *m_pMeshBuffer;
    
    /** size of the skybox*/
    float32 m_fSize;
    
    /** transformation matrix*/
    mat4f m_mTransformationMatrix;
    
};

#endif
