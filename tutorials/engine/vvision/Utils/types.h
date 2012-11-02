/* types.h
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

#ifndef VVISION_base_h
#define VVISION_base_h

#include "vmath.h"
#import <stdlib.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
using namespace std;

namespace vvision
{
    
    /** different primitive type*/
    enum PRIMITIVE_TYPE
    {
        /** triangles*/
        kPRIMITIVE_TYPE_TRIANGLE = GL_TRIANGLES,
        
        /** triangles trip*/
        kPRIMITIVE_TYPE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        
        /** triangles fan*/
        kPRIMITIVE_TYPE_TRIANGLE_FAN = GL_TRIANGLE_FAN,
        
        /** lines*/
        kPRIMITIVE_TYPE_LINES = GL_LINES,
        
        /** line loop*/
        kPRIMITIVE_TYPE_LINE_LOOP = GL_LINE_LOOP,
        
        /** line strip*/
        kPRIMITIVE_TYPE_LINE_STRIP = GL_LINE_STRIP,
        
        /** points*/
        kPRIMITIVE_TYPE_POINTS = GL_POINTS
    };
    
#ifdef ZERO____
    /** determines which attribs will be mapped to the current shadrer ( the attribs used for shadow mapping generator are less than attribs required for rendering)*/
    enum SHADER_VERTEX_ATTRIBUTE
    {
        /** normal attrib location*/
        kSHADER_VERTEX_ATTRIBUTE_NORMAL = 1 << 0,
        
        /** texture attrib location*/
        kSHADER_VERTEX_ATTRIBUTE_TEXTURE = 1 << 1,
        
        /** tangent attrib location*/
        kSHADER_VERTEX_ATTRIBUTE_TANGENT = 1 << 2,
        
        /** color attrib location*/
        kSHADER_VERTEX_ATTRIBUTE_COLOR = 1 << 3
    };
#endif
    
    /** different data type*/
    enum DATA_TYPE
    {
        /** byte*/
        kDATA_TYPE_BYTE = GL_BYTE,
        
        /** unsigned byte */
        kDATA_TYPE_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        
        /** float 32 bits*/
        kDATA_TYPE_FLOAT = GL_FLOAT
    };
    
    /** different gl buffer types*/
    enum GL_BUFFER_TYPE
    {
        /** array buffer*/
        kGL_BUFFER_TYPE_ARRAY = GL_ARRAY_BUFFER,
        
        /** element buffer*/
        kGL_BUFFER_TYPE_ELEMENT = GL_ELEMENT_ARRAY_BUFFER
    };
    
    /** hint for the gl driver on how if the buffer is static of updated frequently*/
    enum GL_BUFFER_USAGE_HINT
    {
        /** static buffer */
        kGL_BUFFER_USAGE_HINT_STATIC = GL_STATIC_DRAW,
        
        /** dynamic*/
        kGL_BUFFER_USAGE_HINT_DYNAMIC = GL_DYNAMIC_DRAW,
        
    };
    
    
    /** shader attribute index*/
    enum
    {
        /** vertex attrib*/
        ATTRIB_VERTEX,
        
        /** color attrib*/
        ATTRIB_COLOR,
        
        /** normal attrib*/
        ATTRIB_NORMAL,
        
        /** texture 0 coord attrib*/
        ATTRIB_TEXTURE0,
        
        /** texture 1 coord attrib*/
        ATTRIB_TEXTURE1,
        
        /** tangent coord attrib*/
        ATTRIB_TANGENT,
        
        /** bone index attrib*/
        ATTRIB_BONES_INDICES,
        
        /** bone weight attrib*/
        ATTRIB_BONES_WEIGHTS,
        
        /** attrib number*/
        NUM_ATTRIBUTES
    };
    
    /* 8 bit unsigned variable.*/
    typedef unsigned char		uchar8;
    
    /* 8 bit signed variable.*/
    typedef signed char		schar8;
    
    /** 8 bit character variable. */
    typedef char			char8;
    
    /** 16 bit unsigned variable. */
    typedef unsigned short		ushort16;
    
    /** 16 bit signed variable.*/
    typedef signed short		sshort16;
    
    /** 32 bit unsigned variable.*/
    typedef unsigned int		uint32;
    
    /** 32 bit signed variable.*/
    typedef signed int		sint32;
    
    /** 32 bit signed variable.*/
    typedef  int		int32;
    
    /** 32 bit floating point variable.*/
    typedef float				float32;
    
    /** 64 bit floating point variable.*/
    typedef double				float64;
    
#define SAFE_DELETE(ptr) \
if(ptr != NULL) \
{delete ptr; ptr = NULL;}
    //maximum finite value
#define VV_MAXIMUM_FLOAT        std::numeric_limits<float32>::max( )
#define VV_MAXIMUM_DOUBLE       std::numeric_limits<float64>::max( )
#define VV_MAXIMUM_INTEGER      std::numeric_limits<int32>::max( )
#define VV_MAXIMUM_CHAR        std::numeric_limits<char8>::max( )
#define VV_MAXIMUM_SHOR_INT    std::numeric_limits<sshort16>::max( )

}

#endif
