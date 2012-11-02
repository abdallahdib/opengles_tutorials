/*
 *  vvisionAppDelegate.mm
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#import "vvisionAppDelegate.h"
#import "vvisionViewController.h"

@implementation vvisionAppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;

- (void)applicationWillResignActive:(UIApplication *)application
{
    [self.viewController stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	NSLog(@"app became active");
    [self.viewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [self.viewController stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Handle any background procedures not related to animation here.
	NSLog(@"app entered background");
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Handle any foreground procedures not related to animation here.
}

- (void)dealloc
{
    self.viewController = nil;//this should invoke a release msg on the retained object ( if count = 0 dealloc is invoked)
    self.window = nil;
    [super dealloc];
}

@end
