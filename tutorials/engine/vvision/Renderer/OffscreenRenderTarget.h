/* OffscreenRenderTarget.h
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

#ifndef VVISION_Offscreen_Render_Target_h
#define VVISION_Offscreen_Render_Target_h

#include "FrameBufferObject.h"
#include "RenderBuffer.h"

namespace vvision
{
    
    enum CONTEXT_TYPE
    {
        /** rgb */
        kCONTEXT_COLOR = 0x01,
        
        /** depth context*/
        kCONTEXT_DEPTH = 0x02,
        
        /** stencil ( not implemented )*/
        kCONTEXT_STENCIL = 0x04,
    };
    
    /**
     offscreen render target, when enabled all OpenGL drawing command will go through this context, offscreen rendering is useful for post processing effects, shadow mapping, reflections, and gpgpu... this offscreen render target doesnt support MRT, u can only attach a texture to the GL_COLOR_ATTACHMENT0 point and a depth buffer to the GL_DEPTH_ATTACHMENT point.
     */
    class COffscreenRenderTarget
    {
        public :
        
        /** constructor create an offscreen render target
         */
        COffscreenRenderTarget();
        
        /** destructor*/
        ~COffscreenRenderTarget();
        
        /** ( intialiaze offscreen rendering) u should at least create an offscreen target with a color buffer attached
         if kCONTEXT_COLOR passed a color texture will be attached to the GL_COLOR_ATTACHMENT0 point
         if kCONTEXT_DEPTH passed a depth render buffer will be attached GL_DEPTH_ATTACHMENT point
         */
        bool Initialize(uint32 context_type = kCONTEXT_COLOR | kCONTEXT_DEPTH, uint32 context_size = 512);
        
        /** return the attached color buffer*/
        inline CTexture* GetColorBuffer() {return m_pColor;}
        
        /** activate context ( all drawing commands will be executed into this context*/
        void Enable();
        
        /** disable context and switch back to default context ( the default context is one bound before Enable was called, this is retrieved via glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_uCurrentFbo); for better performance, it is better to remove the glGet from the rendering loop ( the case when u know the fbo ID )*/
        void Disable();
    private:
        
        /** not permitted*/
        COffscreenRenderTarget(const COffscreenRenderTarget& sm);
        COffscreenRenderTarget& operator=(const COffscreenRenderTarget& sm);
        
        void CleanUp();
        uint32 m_iContextSize;
        
        CFrameBufferObject* m_pFbo;
        
        CRenderBuffer* m_pDepth;
        CTexture* m_pColor;
        
        /** current bound fbo */
        int32 m_uCurrentFbo;
        
        /** default viewport*/
        int32 m_vViewport[4];
    };
}


#endif
