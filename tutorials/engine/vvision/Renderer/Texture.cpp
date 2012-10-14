/*
 *  Texture.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */


#include "Texture.h"

namespace vvision
{
    
    CTexture::CTexture()
    {
        m_uID = 0;
        
    }
    CTexture::~CTexture()
    {
        Release();
    }
    void CTexture::CreateTexture(GLenum target, uint32 width, uint32 height, GLint internal_format,
                                 GLenum host_data_type ,
                                 GLenum host_data_format ,
                                 void *host_data ,
                                 GLint Swrap_mode ,
                                 GLint Twrap_mode ,
                                 GLint min_filter ,
                                 GLint mag_filter,
                                 bool buildMipMap ,
                                 GLint mip_level,
                                 GLint border,
                                 bool enableAnistorpicFiltering
                                 )
    {
        
        m_InternalFormat = internal_format;
        m_uWidth = width;
        m_uheight = height;
        m_eTarget = target;
        m_iSWrapMode = Swrap_mode;
        m_iTWrapMode = Twrap_mode;
        m_iMinFilter = min_filter;
        m_iMagFilter = mag_filter;
        
        glGenTextures(1, &m_uID);
        
        {
            // Allocate and bind an OpenGL texture
            glBindTexture (m_eTarget, m_uID);
            
            // Set desired wrapping type
            glTexParameteri(m_eTarget, GL_TEXTURE_WRAP_S, m_iSWrapMode);
            glTexParameteri(m_eTarget, GL_TEXTURE_WRAP_T, m_iTWrapMode);
            
            if(enableAnistorpicFiltering)
            {
                GLfloat maxAnisotrpoicfiltering;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotrpoicfiltering);
                glTexParameterf(m_eTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotrpoicfiltering);
            }
            
            // Set interpolation function t	o nearest-neighbor
            glTexParameteri(m_eTarget, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
            glTexParameteri(m_eTarget, GL_TEXTURE_MAG_FILTER, m_iMagFilter);
            
            // Set texture format and component count
            glTexImage2D (m_eTarget,
                          mip_level,
                          internal_format,
                          width,
                          height,
                          border,
                          host_data_format,
                          host_data_type,
                          host_data);
            
            if(buildMipMap)
                glGenerateMipmap(m_eTarget);//GL_LINEAR_MIPMAP_LINEAR
        }
    }
    void CTexture::Release()
    {
        if(IsValid())
            glDeleteTextures(1, &m_uID);
        
    }
    bool CTexture::IsValid()
    {
        return glIsTexture(m_uID);
        
    }
    void CTexture::ActivateAndBind(GLenum texture_unit)
    {
        glActiveTexture(texture_unit);
        glBindTexture(m_eTarget, m_uID);
    }
    void CTexture::ActivateAndRefresh(void * data, GLsizei width, GLsizei height, GLint xoffset , GLint yoffset , GLint level , GLenum host_format , GLenum host_type)
    {
        glBindTexture(m_eTarget, m_uID);
        glTexSubImage2D(m_eTarget, level, xoffset, yoffset, width, height, host_format, host_type, data);
        
    }
}