/*
 *  TextureCubeMap.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "TextureCubeMap.h"

namespace vvision
{
    CTextureCubeMap::CTextureCubeMap()
    {
        m_uID = 0;
    }
    CTextureCubeMap::~CTextureCubeMap()
    {
        Release();
    }
    void CTextureCubeMap::CreateTexture(GLenum target, uint32 width, uint32 height, GLint internal_format,
                                        GLenum host_data_type ,
                                        GLenum host_data_format ,
                                        void *host_data_xp,
                                        void *host_data_xn,
                                        
                                        void *host_data_yp,
                                        void *host_data_yn ,
                                        
                                        void *host_data_zp,
                                        void *host_data_zn,
                                        
                                        GLint Swrap_mode ,
                                        GLint Twrap_mode ,
                                        GLint min_filter ,
                                        GLint mag_filter,
                                        bool buildMipMap,
                                        GLint mip_level,
                                        GLint border)
    {
        
        m_iInternalFormat = internal_format;
        m_uWidth = width;
        m_uHeight = height;
        m_eTarget = target;
        
        m_iSWrapMode = Swrap_mode;
        m_iTWrapMode = Twrap_mode;
        m_iMinFilter = min_filter;
        m_iMagFilter = mag_filter;
        
        glGenTextures(1, &m_uID);
        {
            // Allocate and bind an OpenGL texture
            glBindTexture (m_eTarget, m_uID);
            
            // Set interpolation function t	o nearest-neighbor
            glTexParameteri(m_eTarget, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
            glTexParameteri(m_eTarget, GL_TEXTURE_MAG_FILTER, m_iMagFilter);
            
            // Set desired wrapping type
            glTexParameteri(m_eTarget, GL_TEXTURE_WRAP_S, m_iSWrapMode);
            glTexParameteri(m_eTarget, GL_TEXTURE_WRAP_T, m_iTWrapMode);
            
            // Set texture format and component count
            glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                          mip_level,
                          internal_format,
                          width,
                          height,
                          border,
                          host_data_format,
                          host_data_type,
                          host_data_xp);
            
            glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                          mip_level,
                          internal_format,
                          width,
                          height,
                          border,
                          host_data_format,
                          host_data_type,
                          host_data_xn);
            
            glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                          mip_level,
                          internal_format,
                          width,
                          height,
                          border,
                          host_data_format,
                          host_data_type,
                          host_data_yp);
            
            glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                          mip_level,
                          internal_format,
                          width,
                          height,
                          border,
                          host_data_format,
                          host_data_type,
                          host_data_yn);
            
            glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                          mip_level,
                          internal_format,
                          width,
                          height,
                          border,
                          host_data_format,
                          host_data_type,
                          host_data_zp);
            
            glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                          mip_level,
                          internal_format,
                          width,
                          height,
                          border,
                          host_data_format,
                          host_data_type,
                          host_data_zn);
            
            glBindTexture (m_eTarget, 0);
            
            if(buildMipMap)
                glGenerateMipmap(GL_TEXTURE_2D);//GL_LINEAR_MIPMAP_LINEAR
        }
    }
    void CTextureCubeMap::Release()
    {
        if(IsValid())
            glDeleteTextures(1, &m_uID);
        
    }
    bool CTextureCubeMap::IsValid()
    {
        return glIsTexture(m_uID);
        
    }
    void CTextureCubeMap::ActivateAndBind(GLenum texture_unit)
    {
        glActiveTexture(texture_unit);
        glBindTexture(m_eTarget, m_uID);
    }
    void CTextureCubeMap::ActivateAndRefresh(GLenum target,void * data, GLsizei width, GLsizei height, GLint xoffset , GLint yoffset , GLint level , GLenum host_format , GLenum host_type)
    {
        glBindTexture(m_eTarget, m_uID);
        glTexSubImage2D(target, level, xoffset, yoffset, width, height, host_format, host_type, data);
        
    }
}
