/* Mesh.h
 *
 * Virtual Vision Engine . Copyright (C) 2012 Abdallah DIB.
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

#ifndef VVISION_Mesh_h
#define VVISION_Mesh_h


#include "renderer.h"

namespace vvision
{
    
    class IMesh
    {
    public:
        /** constructor*/
        IMesh()
        {
            m_pMeshBuffer = new CMeshBuffer();
        }
        
        /** destructor*/
        virtual ~IMesh() {SAFE_DELETE(m_pMeshBuffer);}
        
        /** return the loaded mesh*/
        inline CMeshBuffer* GetMeshBuffer() {return m_pMeshBuffer;}
        
        /** clear/delete cached resources*/
        virtual void DeleteCachedResources() = 0;
        
        /** load mesh ( use this to load ur mesh)*/
        static IMesh* LoadMeshFromFile(const string& meshName);
    protected:
        
        /**load a mesh
         * @FileName name of the file to load
         * @retrun true if success
         */
        virtual bool LoadMesh(const std::string& FileName) = 0;
        
        CMeshBuffer* m_pMeshBuffer;
        
    private:
        /** not allowed*/
        IMesh(const IMesh&);
        
        /** not allowed*/
        IMesh& operator=(const IMesh& mesh);
        
    };
}
#endif
