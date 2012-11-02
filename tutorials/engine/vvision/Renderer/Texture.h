/* Texture.h
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

#ifndef GL_TEXTURE_INCLUDED
#define GL_TEXTURE_INCLUDED

#include "types.h"

namespace vvision
{
    
    class CTexture
    {
        
        public :
        
        /** consturctor*/
        CTexture();
        
        /** destructor*/
        ~CTexture();
        
        /** create a 2D texture*/
        void CreateTexture(GLenum target, uint32 width, uint32 height, GLint internal_format,
                           GLenum host_data_type ,
                           GLenum host_data_format ,
                           void *host_data = 0,
                           GLint Swrap_mode = GL_CLAMP_TO_EDGE,
                           GLint Twrap_mode = GL_CLAMP_TO_EDGE,
                           GLint min_filter = GL_LINEAR,
                           GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR,
                           bool buildMipMap = true,
                           GLint mip_level = 0,
                           GLint border = 0,
                           bool enableAnistorpicFiltering = true
                           );
        /** check if the texture is valid */
        bool IsValid();
        
        /** bind texture to the given texture unit*/
        void ActivateAndBind(GLenum texture_unit);
        
        /** activate and refresh texture*/
        void ActivateAndRefresh(void * data, GLsizei width, GLsizei height, GLint xoffset = 0, GLint yoffset = 0,GLint level = 0, GLenum host_format = GL_BGRA, GLenum host_type = GL_UNSIGNED_BYTE);
        
        /** release texture*/
        void Release();
        
        /** get width*/
        inline uint32 GetWidth() {return m_uWidth;}
        
        /** get height*/
        inline uint32 GetHeight() {return m_uheight;}
        
        /** get texture id*/
        inline uint32 GetID() {return m_uID;}
        
        /** get texture internal format*/
        inline GLint GetInternalFormat() {return m_InternalFormat;}
        
        /** get texture target*/
        inline GLenum GetTarget() {return m_eTarget;}
        
    private:
        
        /** not allowed*/
        CTexture(const CTexture &p);
        
        /** not allowed*/
        CTexture& operator =(const CTexture &p);
        
        /** texture width*/
        uint32 m_uWidth;
        
        /** texture height*/
        uint32 m_uheight;
        
        /** texture internal format*/
        GLint m_InternalFormat;
        
        /** GL ID*/
        GLuint m_uID;
        
        /** target type ( texture 2D)*/
        GLenum m_eTarget;
        
        /** S wrap mode*/
        GLint m_iSWrapMode;
        
        /** T wrap mode*/
        GLint m_iTWrapMode;
        
        /** mignification filter*/
        GLint m_iMinFilter;
        
        /** magnification filter*/
        GLint m_iMagFilter;
        
    };
}

#endif
