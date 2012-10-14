/*
 *  EAGLView.m
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"


@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation EAGLView

@synthesize framebufferWidth;
@synthesize framebufferHeight;
@dynamic context;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

-(BOOL)isRetina
{
    return ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && ([UIScreen mainScreen].scale == 2.0))?1:0;
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
	if (self)
    {
        
#ifdef VVISION_ENABLE_SCREEN_RETINA
        
        // Retina display detected*/
        float scaleFactor = VVISION_RETINA_SCALE_FACTOR;
		if([self isRetina])
        {
            NSLog(@"EAGLView: [INFO] detected retina display: scaling to %f", scaleFactor);
			// Set contentScale Factor to 2
			self.contentScaleFactor = scaleFactor;
			// Also set our glLayer contentScale Factor to 2
			CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
			eaglLayer.contentsScale = scaleFactor; //new line
		}
#endif
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
    }
    
    return self;
}

- (void)dealloc
{
    [self deleteFramebuffer];    
    [context release];
    
    [super dealloc];
}

- (EAGLContext *)context
{
    return context;
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext)
    {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

#ifdef VVISION_ENABLE_MSAA
-(void) createMsaaBuffer
{    
    glGenFramebuffers(1, &msaaFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
    
    glGenRenderbuffers(1, &msaaRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderBuffer);
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 2, GL_RGBA8_OES, framebufferWidth, framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaRenderBuffer);
    
    glGenRenderbuffers(1, &msaaDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthBuffer);
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 2, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepthBuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    
}
#endif

-(void) createResolveBuffer
{
    
    // Create default framebuffer object.
    glGenFramebuffers(1, &defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    // Create color render buffer and allocate backing store.
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    
    //re bind color render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
}
- (void)createFramebuffer
{
    if (context && !defaultFramebuffer)
    {
        [EAGLContext setCurrentContext:context];
        [self createResolveBuffer];
    }
    
#ifdef VVISION_ENABLE_MSAA
    if(context && !msaaFramebuffer)
    {
        //create msaa buffer
        [self createMsaaBuffer];
    }
#endif    
}

- (void)deleteFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer)
        {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer)
        {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
		
		if (depthRenderBuffer)
        {
            glDeleteRenderbuffers(1, &depthRenderBuffer);
            depthRenderBuffer = 0;
        }
        
        //MSAA
#ifdef VVISION_ENABLE_MSAA
        if (msaaFramebuffer)
        {
            glDeleteFramebuffers(1, &msaaFramebuffer);
            msaaFramebuffer = 0;
        }
        
        if (msaaRenderBuffer)
        {
            glDeleteRenderbuffers(1, &msaaRenderBuffer);
            msaaRenderBuffer = 0;
        }
		
		if (msaaDepthBuffer)
        {
            glDeleteRenderbuffers(1, &msaaDepthBuffer);
            msaaDepthBuffer = 0;
        }
#endif
        
    }
}

- (void)setFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
#ifdef VVISION_ENABLE_MSAA
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
#else
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
#endif
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
#ifdef VVISION_ENABLE_MSAA
        //Bind both MSAA and View FrameBuffers. 
        glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer);  
        glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, msaaFramebuffer);
        // Call a resolve to combine both buffers 
        glResolveMultisampleFramebufferAPPLE();
#endif
        
        
#ifdef VVISION_ENABLE_DISCARD_FRAME_BUFFER 
        //supported on IOS 4.0 and up
        static GLenum discards[]  = {GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0};
        glDiscardFramebufferEXT(GL_FRAMEBUFFER, 2, discards);
#endif
        
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

@end
