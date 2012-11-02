/*
 *  GPUManager.mm
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#import "GPUManager.h"
#import "CacheResourceManager.h"
@implementation GPUManager

@synthesize  context = _context;
@synthesize contextWidth = _contextWidth;
@synthesize contextHeight = _contextHeight;


CCacheResourceManager& crm_ = CCacheResourceManager::Instance();

-(id) init
{
    self = [super init];
    
    if(self)
    {
        _tutorial = new Tutorial();
	}
	
	return self;
}

- (BOOL) CreateOpenGLESContext
{
    if (self.context != nil)
        return YES;
    
	EAGLContext * aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!aContext)
	{
		NSLog(@"Failed to create ES context");
		return FALSE;
	}
    
	else if (![EAGLContext setCurrentContext:aContext])
	{
		NSLog(@"Failed to set ES context current");
		return FALSE;
	}
	
	self.Context = aContext;
	[aContext release];
	
    
    return _tutorial->Deploy();
}	

- (void) AttachViewToContext:(UIView*) view
{
	if (view != nil) 
	{
		_glview = (EAGLView*)view;
		[_glview setContext:self.context];
		[_glview setFramebuffer];
		self.contextWidth = [ _glview framebufferWidth ];
		self.contextHeight = [ _glview framebufferHeight ];
	}
}
- (void)dealloc
{
	[self TearOpenGLESContext];
    self.context = nil;
    SAFE_DELETE(_tutorial);
	[super dealloc];	
}
- (void) TearOpenGLESContext
{
    // Tear down context.
    if ([EAGLContext currentContext] == self.context)
        [EAGLContext setCurrentContext:nil];		
}

- (void)DrawFrame
{
    if(self.context)
	{
        [EAGLContext setCurrentContext:self.context];
        [_glview setFramebuffer];
    }
    
    //draw frame
    _tutorial->Frame();
    [_glview presentFramebuffer];
    
    
}
-(void) receivedControl:(CControlPad&) pad
{
    _tutorial->UpdateControls(pad);
}
@end
