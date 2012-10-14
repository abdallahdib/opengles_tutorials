/*
 *  RenderBuffer.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "RenderBuffer.h"
namespace vvision
{
    
    CRenderBuffer::CRenderBuffer()
    :m_uID(GenerateId()),m_iSaveID(0)
    {
    }
    CRenderBuffer::~CRenderBuffer()
    {
        glDeleteRenderbuffers(1,&m_uID);
        
    }
    void CRenderBuffer::Bind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_uID);
    }
    void CRenderBuffer::UnBind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    bool CRenderBuffer::SetParameters(GLenum internalFormat, int32  width, int32 height)
    {
        int maxSize = CRenderBuffer::GetMaximumSize();
        if (width > maxSize || height > maxSize )
        {
            std::cerr << "Renderbuffer::SetParameters() ERROR:\n\t"
            << "Size too big (" << width << ", " << height << ")\n";
            
            return false;
        }
        _GuardedBind();
        // Allocate memory for renderBuffer
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height );
        _GuardedUnbind();
        
        return true;
        
    }
    bool CRenderBuffer::_GuardedBind()
    {
        // Only binds if m_render buffer id  is different than the currently bound to render buffer
        glGetIntegerv( GL_RENDERBUFFER_BINDING, &m_iSaveID );
        if (m_uID != (GLuint)m_iSaveID) {
            Bind();
            return true;
        } else return false;
    }
    
    bool CRenderBuffer::_GuardedUnbind()
    {
        
        if (m_uID != (GLuint)m_iSaveID) {
            glBindRenderbuffer(GL_RENDERBUFFER, (GLuint)m_iSaveID);
            return true;
        } return false;
    }
    int32 CRenderBuffer::GetMaximumSize()
    {
        GLint maxAttach = 0;
        glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, &maxAttach );
        return maxAttach;
    }
    GLuint CRenderBuffer::GenerateId()
    {
        GLuint id = 0;
        glGenRenderbuffers(1, &id);
        return id;
    }
}

