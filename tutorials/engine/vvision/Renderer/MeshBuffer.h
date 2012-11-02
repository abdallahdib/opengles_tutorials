/* MeshBuffer.h
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

#ifndef CMESH_GROUP_INCLUDED
#define CMESH_GROUP_INCLUDED

#include "VertexBuffer.h"
#include "Material.h"
#include "BoundingBox.h"
#include "Shader.h"
#include "types.h"

namespace vvision
{
    /** represents a gpu vertex*/
    struct CGpuVertex {
        
        /** vertex pos*/
        vec3f pos;
        
        /** vertex tex cooridnate*/
        vec2f texCoord;
        
        /** vertex normal*/
        vec3f normal;
        
        /** bone indices that affect this vertex ( for animated mesh only)*/
        vec4f boneIndices;
        
        /** bone weights that affect this vertex for animated mes only)*/
        vec4f boneWeights;
        
        /** vertex tangent coordinate*/
        vec3f tangent;
        
        /** constructor*/
        CGpuVertex() :
        pos(vec3f()), normal(vec3f()), tangent(vec3f()), texCoord(vec2f()), boneIndices(vec4f(0,0,0,0)), boneWeights(vec4f(0,0,0,0))
        {}
        CGpuVertex(const vec3f& pos_, const vec3f& normal_, const vec3f& tangent_, const vec2f& texCoord_, const vec4f& boneIndices_, const vec4f& boneWeights_)
        :
        pos(pos_), normal(normal_), tangent(tangent_), texCoord(texCoord_), boneIndices(boneIndices_), boneWeights(boneWeights_)
        {
        }
        
        /** copy constructor*/
        CGpuVertex(const CGpuVertex& v)
        {
            pos = v.pos;
            normal = v.normal;
            tangent = v.tangent;
            texCoord = v.texCoord;
            boneIndices = v.boneIndices;
            boneWeights = v.boneWeights;
        }
        
        /** = operator*/
        CGpuVertex& operator=(CGpuVertex const& v)
        {
            pos = v.pos;
            normal = v.normal;
            tangent = v.tangent;
            texCoord = v.texCoord;
            boneIndices = v.boneIndices;
            boneWeights = v.boneWeights;
            return *this;
        }
        };
        
        /** a mesh is represented by a number of MeshGroup, mesh groups can share materials
         * a mesh group is stored on GPU using vbo + vao
         */
        class CMeshGroup
        {
            
        public:
            
            /** constructor*/
            CMeshGroup()
            : m_iMaterialIndex(-1), m_cBbox(GetBoundingBox()), m_uAttribFlags(0), m_eDrawingMode(kPRIMITIVE_TYPE_TRIANGLE)
            {
            }
            
            /** destructor*/
            ~CMeshGroup();
            
            /** get drawing type*/
            inline PRIMITIVE_TYPE GetDrawingMode() {return m_eDrawingMode;}
            
            /** return a reference to containers */
            inline vector<CGpuVertex>& GetVertices() {return m_vVertices;}
            inline vector<ushort16>& GetIndices(){return m_vIndices;}
            
            /** getters/setters*/
            inline void SetMaterialIndex(int32 matIndex) {m_iMaterialIndex = matIndex;}
            inline int32 GetMaterialIndex() {return m_iMaterialIndex;}
            inline CBoundingBox& GetBoundingBox() {return m_cBbox;}
            inline void SetBoundingBox(const CBoundingBox& bbox_) {m_cBbox = bbox_;}
            
            
            /** create bounding box for the mesh*/
            void CreateBoundingBox()
            {
                for(int32 i= 0; i< m_vVertices.size();i++)
                    m_cBbox.Add(m_vVertices[i].pos);
            }
            
            /** allocate mesh on gpu*/
            bool AllocateOnGpuMemory( GL_BUFFER_USAGE_HINT usage_)
            {
                glGenVertexArraysOES(1, &m_uVao);
                glBindVertexArrayOES(m_uVao);
                
                bool res = m_cVboVertices.AllocateStorage(kGL_BUFFER_TYPE_ARRAY, usage_, sizeof(CGpuVertex) * m_vVertices.size(), &m_vVertices[0]);
                if(!res)
                    return false;
                
                res = m_cVboIndices.AllocateStorage(kGL_BUFFER_TYPE_ELEMENT, usage_, sizeof(ushort16) * m_vIndices.size(), &m_vIndices[0]);
                if(!res)
                    return false;
                
                m_cVboVertices.Bind();
                CVertexBuffer::MapAttribLocation(ATTRIB_VERTEX,         3, kDATA_TYPE_FLOAT, sizeof(CGpuVertex), 0);
                CVertexBuffer::MapAttribLocation(ATTRIB_TEXTURE0,       2, kDATA_TYPE_FLOAT, sizeof(CGpuVertex), (const GLvoid*)12);
                CVertexBuffer::MapAttribLocation(ATTRIB_NORMAL,         3, kDATA_TYPE_FLOAT, sizeof(CGpuVertex), (const GLvoid*)20);
                CVertexBuffer::MapAttribLocation(ATTRIB_BONES_INDICES,  4, kDATA_TYPE_FLOAT, sizeof(CGpuVertex), (const GLvoid*)32);
                CVertexBuffer::MapAttribLocation(ATTRIB_BONES_WEIGHTS,  4, kDATA_TYPE_FLOAT, sizeof(CGpuVertex), (const GLvoid*)48);
                CVertexBuffer::MapAttribLocation(ATTRIB_TANGENT,        3, kDATA_TYPE_FLOAT, sizeof(CGpuVertex), (const GLvoid*)64);
                
                m_cVboIndices.Bind();
                m_cVboVertices.Unbind();
                glBindVertexArrayOES(0);
                return true;
                
            }
            
            /** map group to gpu ( prepare for rendering mesh group)
             * @param flags check e_ATTRIB_PARAM in GL_Enum.h
             */
            void MapToGPU(uint32 flags)
            {
                glBindVertexArrayOES(m_uVao);
            }
            
            /** unmap*/
            void UnmapFromGPU()
            {
                glBindVertexArrayOES(0);
            }
            
        private:
            
            /** drawing mode*/
            PRIMITIVE_TYPE m_eDrawingMode;
            
            /** attrib flag*/
            uint32 m_uAttribFlags;
            
            /** vbo*/
            CVertexBuffer m_cVboVertices;
            CVertexBuffer m_cVboIndices;
            
            /** vao*/
            GLuint m_uVao;
            
            /** vertices representing this meshGroupd*/
            std::vector<CGpuVertex> m_vVertices;
            
            /** indices of the mesh group*/
            std::vector<ushort16> m_vIndices;
            
            /** material index (meshGroup can share  the same material)*/
            int32 m_iMaterialIndex;
            
            /** the bounding box */
            CBoundingBox m_cBbox;
            
            /** copy constructor*/
            CMeshGroup(const CMeshGroup& m)
            {
                m_iMaterialIndex = m.m_iMaterialIndex;
                m_vVertices = m.m_vVertices;
                m_vIndices = m.m_vIndices;
                m_cBbox = m.m_cBbox;
            }
            
            /** = operator*/
            CMeshGroup& operator=(CMeshGroup const& m)
            {
                
                m_iMaterialIndex = m.m_iMaterialIndex;
                m_vVertices = m.m_vVertices;
                m_vIndices = m.m_vIndices;
                m_cBbox = m.m_cBbox;
                return *this;
            }
            
            
        };
        
        /** conatainer of groups and materials, */
        class CMeshBuffer
        {
            
            /** public attributes*/
        public:
            /** constructor*/
            CMeshBuffer() {}
            
            /**destructor*/
            ~CMeshBuffer();
            
            /** groups count*/
            inline uint32 GroupsCount() {return groups.size();}
            
            /** materials count */
            inline uint32 MaterialsCount() {return materials.size();}
            
            /** group at index */
            inline CMeshGroup* GroupAtIndex(uint32 index) {assert(index < groups.size()); return groups[index];}
            
            /** material for group*/
            inline CMaterial* MaterialForGroup(CMeshGroup* grp) {return materials[grp->GetMaterialIndex()];}
            
            /** material at index*/
            inline CMaterial* MaterialAtindex(uint32 index) {assert(index < materials.size()); return materials[index];}
            
            /** add group*/
            inline void AddGroup(CMeshGroup* grp) {assert(grp != NULL); groups.push_back(grp);}
            
            /** add material*/
            inline void AddMaterial(CMaterial* mat) {assert(mat != NULL); materials.push_back(mat);}
            
            /** reference to materials*/
            std::vector<CMaterial*>& GetMaterialsContainerRef() {return materials;}
            
            /**reference to groups*/
            std::vector<CMeshGroup*>& GetGroupsContainerRef() {return groups;}
            
        private:
            
            /** groups container*/
            std::vector<CMeshGroup*> groups;
            
            /** materials container*/
            std::vector<CMaterial*> materials;
            
            /** not allowed*/
            CMeshBuffer(const CMeshBuffer& m);
            
            /** not allowed*/
            CMeshBuffer& operator = (CMeshBuffer const& m);
        };
    }
    
#endif