/* vvisionViewController.h
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#import "GPUManager.h"

@interface vvisionViewController : UIViewController
{	

	GPUManager* _gpuManager;
    
    BOOL animating;
    NSInteger _animationFrameInterval;
    CADisplayLink *_displayLink;
}

@property (nonatomic, assign) CADisplayLink *displayLink;

- (void)startAnimation;
- (void)stopAnimation;

@end
