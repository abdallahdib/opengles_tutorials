/*
 *  OffscreenRenderTarget.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "OffscreenRenderTarget.h"

namespace vvision
{
    COffscreenRenderTarget::COffscreenRenderTarget()
    :
    m_iContextSize(512), m_pFbo(NULL), m_pDepth(NULL), m_pColor(NULL),
    m_uCurrentFbo(0)
    {
    }
    
    COffscreenRenderTarget::~COffscreenRenderTarget()
    {
        CleanUp();
    }
    
    void COffscreenRenderTarget::CleanUp()
    {
        SAFE_DELETE(m_pFbo);
        SAFE_DELETE(m_pDepth);
        SAFE_DELETE(m_pColor);
    }
    bool COffscreenRenderTarget::Initialize(uint32 context_type, uint32 context_size)
    {
        CleanUp();
        m_iContextSize = context_size;
        
        
        //create the frame buffer object and bind it !
        m_pFbo = new CFrameBufferObject();
        m_pFbo->CreateID();
        m_pFbo->Bind();
        
        
        if (context_type & kCONTEXT_COLOR)
        {
            m_pColor=new CTexture();
            m_pColor->CreateTexture(GL_TEXTURE_2D, m_iContextSize, m_iContextSize, GL_RGBA,
                                    GL_UNSIGNED_BYTE, GL_RGBA, NULL, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, true, 0, 0);
            m_pFbo->AttachTexture(*m_pColor,GL_COLOR_ATTACHMENT0);
            
        }
        
        if(context_type & kCONTEXT_DEPTH)
        {
            m_pDepth = new CRenderBuffer();
            if(!m_pDepth->SetParameters(GL_DEPTH_COMPONENT16, m_iContextSize, m_iContextSize))
                return false;
            
            m_pDepth->Bind();
            m_pFbo->AttachRenderBuffer(m_pDepth->GetID(),GL_DEPTH_ATTACHMENT);
        }
        
        
        //check fbo for completeness
        if(! CFrameBufferObject::CheckStatus())
            return false;
        
        //disabel offscreen context
        m_pFbo->Disable();
        
        //
        return true;
    }
    void COffscreenRenderTarget::Enable()
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_uCurrentFbo);
        glGetIntegerv(GL_VIEWPORT, m_vViewport);
        m_pFbo->Bind();
        glViewport(0, 0, m_iContextSize, m_iContextSize);
    }
    
    void COffscreenRenderTarget::Disable()
    {
        m_pFbo->Disable();
        glBindFramebuffer(GL_FRAMEBUFFER, m_uCurrentFbo);
        glViewport(m_vViewport[0], m_vViewport[1], m_vViewport[2], m_vViewport[3]);
        //glViewport(m_iVpX, m_iVpY, m_iVpW, m_iVpH);
    }
}
