/* GPUManager.h
 *
 * Virtual Vision Engine . Copyright (C) 2012 Abdallah DIB.
 * All rights reserved. Email: Abdallah.dib@virtual-vison.net
 * Web: <http://www.virutal-vision.net/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#import <Foundation/Foundation.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <QuartzCore/QuartzCore.h>
#import "EAGLView.h"
#import "Tutorial.h"

@interface GPUManager : NSObject {
	
	EAGLView* _glview;
	GLuint _contextWidth;
	GLuint _contextHeight;
	EAGLContext *_context;
    Tutorial* _tutorial;
}

@property (nonatomic, retain) EAGLContext *context;
@property GLuint contextWidth;
@property GLuint contextHeight;

- (id) init;
- (void) AttachViewToContext:(UIView*) view;
- (BOOL) CreateOpenGLESContext;
- (void) TearOpenGLESContext;
- (void)DrawFrame;
-(void) receivedControl:(CControlPad&) pad;
@end