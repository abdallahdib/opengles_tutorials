/* VertexBuffer.h
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

#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#include "types.h"

namespace vvision
{
    
    /** opengl buffer*/
    class CVertexBuffer
    {
    public:
        
        /** constructor*/
        CVertexBuffer()
        : m_eBufferType(kGL_BUFFER_TYPE_ARRAY), m_eBufferUsage(kGL_BUFFER_USAGE_HINT_STATIC)
        {
        }
        
        /** construct with given buffer type and usage*/
        CVertexBuffer(GL_BUFFER_TYPE bufferType_, GL_BUFFER_USAGE_HINT usage_)
        : m_eBufferType(bufferType_), m_eBufferUsage(usage_)
        {
            
        }
        
        /** destructor*/
        ~CVertexBuffer()
        {
            Destroy();
        }
        
        /** Destroy*/
        void Destroy()
        {
            if(glIsBuffer(m_uVboID))
            {
                glDeleteBuffers(1, &m_uVboID);
                m_uVboID = 0;
            }
        }
        
        /** allocate buffer on gpu*/
        bool AllocateStorage(GL_BUFFER_TYPE bufferType_, GL_BUFFER_USAGE_HINT usage_, int32 storageSizeByte, void* data)
        {
            m_eBufferType = bufferType_;
            m_eBufferUsage = usage_;
            glGenBuffers(1, &m_uVboID);
            
            if(m_uVboID <= 0)
                return false;
            
            glBindBuffer(m_eBufferType, m_uVboID);
            glBufferData(m_eBufferType, storageSizeByte, data, m_eBufferUsage);
            glBindBuffer(m_eBufferType, 0);
            return true;
        }
        
        /** update buffer content*/
        inline void UpdateContent(int32 offset, int32 sizeByte, void* data)
        {
            glBindBuffer(m_eBufferType, m_uVboID);
            glBufferSubData(m_eBufferType, offset, sizeByte, data);
            glBindBuffer(m_eBufferType, 0);
        }
        
        /** bind buffer*/
        inline void Bind()
        {
            glBindBuffer(m_eBufferType, m_uVboID);
        }
        
        /** unbind buffer*/
        inline void Unbind()
        {
            glBindBuffer(m_eBufferType, 0);
        }
        
        /** send attrib to shader */
        static void MapAttribLocation(int32 index, int32 componentsPerElement, DATA_TYPE type, int32 stride, const void* ptr)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, componentsPerElement, type, GL_FALSE, stride, ptr);
        }
        /** desactivate the given attrib*/
        static void UnmapAttribLocation(int32 index)
        {
            glDisableVertexAttribArray(index);
        }
        
    private:
        /** copy constructor no allowed, doenst make sense to copy a mesh buffer ?*/
        CVertexBuffer(const CVertexBuffer& c);
        
        /** = operator no allowed, */
        CVertexBuffer& operator =(const CVertexBuffer& c);
        
        /** buffer id*/
        uint32 m_uVboID;
        
        /** buffer gl hint usage */
        GL_BUFFER_USAGE_HINT m_eBufferUsage;
        
        /** buffer type*/
        GL_BUFFER_TYPE m_eBufferType;
    };
}

#endif