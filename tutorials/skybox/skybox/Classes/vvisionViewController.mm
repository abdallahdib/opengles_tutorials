/*
 *  vvisionViewController.mm
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#import <QuartzCore/QuartzCore.h>

#import "vvisionViewController.h"
#import "EAGLView.h"

@implementation vvisionViewController

#define kKeyUpTag 1000
#define kKeyDownTag 1001
#define kKeyLeftTag 1002
#define kKeyRightTag 1003

#define kKeyMoveForwardTag 1004
#define kKeyMoveBackwardTag 1005

@synthesize  displayLink = _displayLink;

- (void)awakeFromNib
{
	
	
	_gpuManager = [ [GPUManager alloc] init ];
    
	if(![_gpuManager CreateOpenGLESContext])
	{
		NSLog(@"Failed to initialize context or missing resources. Application will exit..");
        cerr.flush();
		exit(EXIT_FAILURE);
	}
    
	[_gpuManager AttachViewToContext:(EAGLView *)self.view];
}

- (void)dealloc
{

	[self cleanup];
	[super dealloc];
}
-(void) cleanup
{ 
    self.displayLink = nil;
	if (_gpuManager != nil)
		[_gpuManager release];
}
- (NSInteger)animationFrameInterval
{
    return _animationFrameInterval;
}



- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1)
    {
        _animationFrameInterval = frameInterval;
        
        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        CADisplayLink *aDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:_animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}
- (void)drawFrame
{
    [_gpuManager DrawFrame];
    [_gpuManager receivedControl:_controlPad];
}
-(void) viewDidLoad
{
	[super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self startAnimation];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self stopAnimation];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
    
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	//return FALSE;
	return (interfaceOrientation == UIInterfaceOrientationLandscapeRight  );
}
-(IBAction)buttonToutchUp:(id)butID
{
    if(butID != nil && [butID isKindOfClass:[UIButton class]])
    {
        int tag = ((UIButton*)butID).tag;
        
        switch (tag) {
            case kKeyUpTag:
                _controlPad.keyUp = false;
                break;
                
            case kKeyDownTag:
                _controlPad.keyDown = false;
                break;
                
            case kKeyLeftTag:
                _controlPad.keyLeft = false;
                break;
                
            case kKeyRightTag:
                _controlPad.keyRight = false;
                break;
                
            case kKeyMoveForwardTag:
                _controlPad.keyMoveUp = false;
                break;
                
            case kKeyMoveBackwardTag:
                _controlPad.keyMoveDown = false;
                break;
            default:
                break;
        }
    }
    
    
    
}
-(IBAction)buttonToutchDown:(id)butID
{
    if(butID != nil && [butID isKindOfClass:[UIButton class]])
    {
        int tag = ((UIButton*)butID).tag;
        
        switch (tag) {
            case kKeyUpTag:
                _controlPad.keyUp = true;
                break;
                
            case kKeyDownTag:
                _controlPad.keyDown = true;
                break;
                
            case kKeyLeftTag:
                _controlPad.keyLeft = true;
                break;
                
            case kKeyRightTag:
                _controlPad.keyRight = true;
                break;
                
            case kKeyMoveForwardTag:
                _controlPad.keyMoveUp = true;
                break;
                
            case kKeyMoveBackwardTag:
                _controlPad.keyMoveDown = true;
                break;
                
            default:
                break;
        }
    }
    
    
}


@end
