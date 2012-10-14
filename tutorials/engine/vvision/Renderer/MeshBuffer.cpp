/*
 *  MeshGroup.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "MeshBuffer.h"
namespace vvision
{
    CMeshGroup::~CMeshGroup()
    {
        m_vVertices.clear();
        m_vIndices.clear();
        m_iMaterialIndex = -1;
        m_cVboVertices.Destroy();
        m_cVboIndices.Destroy();
    }
    
    CMeshBuffer::~CMeshBuffer()
    {
        for (int32 i=0; i<groups.size(); i++) {
            delete groups[i];
            groups[i] = NULL;
        }
        groups.clear();
        
        for (int32 i=0; i<materials.size(); i++)
        {
            delete materials[i];
            materials[i] = NULL;
        }
        materials.clear();
    }
}