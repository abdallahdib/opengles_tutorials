/* EAGLView.h
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

#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

/** set this flag to get better image quality on retina displays*/
#define VVISION_ENABLE_SCREEN_RETINA

/** the screen content scale factor ( float between be between 1. and 2.)
 higher => better image quality better performance
 lower => better performance poor image quality
 */
#define VVISION_RETINA_SCALE_FACTOR 1.8f

/** enable/disable multi sampling anti aliasing ( this will create two render buffers, one for multisampling and the second for resolving */
//#define VVISION_ENABLE_MSAA

/** enable discard frame buffer before presenting frame buffer, this tells the GL driver that the we dont want to modify the frame buffer ( discarding frame buffer is recommended by Apple) */
#define VVISION_ENABLE_DISCARD_FRAME_BUFFER

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView
{
@private
    EAGLContext *context;
    
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLuint defaultFramebuffer, colorRenderbuffer, depthRenderBuffer;
    
#ifdef VVISION_ENABLE_MSAA
    //Buffer definitions for the MSAA 
    GLuint msaaFramebuffer, msaaRenderBuffer, msaaDepthBuffer;
#endif
}

@property (nonatomic, retain) EAGLContext *context;
@property GLint framebufferWidth;
@property GLint framebufferHeight;
- (void)setFramebuffer;
- (BOOL)presentFramebuffer;

@end
