/* vvisionAppDelegate.h
*  Virtual Vision
*
*  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
*  Copyright 2011 Virtual Vision. All rights reserved.
*
*/

#import <UIKit/UIKit.h>

@class vvisionViewController;

@interface vvisionAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *_window;
    vvisionViewController *_viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet vvisionViewController *viewController;

@end

