/* RenderBuffer.h
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

#ifndef GL_REDNER_BUFFER_INCLUDED
#define GL_REDNER_BUFFER_INCLUDED

#include "types.h"

namespace vvision
{
    class CRenderBuffer
    {
        
        public :
        
        /** constructor*/
        CRenderBuffer();
        
        /** destructor*/
        ~CRenderBuffer();
        
        /** bind and activate render buffer*/
        void Bind();
        
        /** unbind and desactivate render buffer*/
        void UnBind();
        
        /** set render buffer format, widht and height
         * return true if success
         */
        bool SetParameters(GLenum InternalFormat,int32 width,int32 height);
        
        /** get gpu buffer ID*/
        inline uint32 GetID() { return m_uID;}
        
        /** static function to determine the maximum available size for the current opengl implementation*/
        static int32 GetMaximumSize();
        
        private :
        /** not allowed*/
        CRenderBuffer(const CRenderBuffer& sm);
        CRenderBuffer& operator=(const CRenderBuffer& sm);
        
        /** generate GL id for the buffe*/
        GLuint GenerateId();
        
        /** guarded bind/unding to avoid unecessary call to opengl API*/
        bool _GuardedBind();
        bool _GuardedUnbind();
        
        /**the buffer gpu ID*/
        GLuint m_uID;
        
        /**used for the guarded bind*/
        GLint m_iSaveID;
        
    };
}
#endif

