/*
 *  Mesh.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */


#include "Mesh.h"
#include "AssimpMesh.h"

namespace vvision
{
    IMesh* IMesh::LoadMeshFromFile(const string& meshName)
    {
        IMesh* mesh = new CAssimpMesh();
        
        if(!mesh->LoadMesh(meshName))
        {
            delete mesh;
            return NULL;
        }
        
        return mesh;
    }
}