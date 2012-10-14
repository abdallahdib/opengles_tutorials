/*
 *  FilePath.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "FilePath.h"
#include <fstream>
#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#import <QuartzCore/QuartzCore.h>
#include "Helpers.h"

namespace vvision
{
    
    string getPath(const char8 *filename)
    {
        char8 *ptr;
        std::string fnm(filename);
        if(fnm.find("/") == fnm.npos)
        {
            CFBundleRef mainBundle = CFBundleGetMainBundle();
            CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
            CFStringRef str = CFURLCopyFileSystemPath(resourcesURL,
                                                      kCFURLPOSIXPathStyle);
            CFRelease(resourcesURL);
            ptr = new char8[CFStringGetLength(str)+1];
            CFStringGetCString(str,
                               ptr,
                               FILENAME_MAX,
                               kCFStringEncodingASCII);
            CFRelease(str);
            
            std::string res(ptr);
            res += std::string("/");
            res += std::string(filename);
            
            delete[] ptr;
            ptr = new char8[res.length()+1];
            strcpy(ptr, res.c_str());
        }
        
        else
        {
            ptr = new char8[fnm.length()+1];
            strcpy(ptr, fnm.c_str());
        }
        
        string s(ptr);
        delete[] ptr;
        return s;
    }
    
    string getPath(const string& filename)
    {
        return getPath(filename.c_str());
    }
    
    string getContentFromPath(const char8 *filepath)
    {
        std::string buf("");
        std::string line("");
        std::ifstream in(filepath);
        
        if(in.fail())
            return "";
        while(std::getline(in,line))
        {
            line += std::string("\n");
            buf += line;
        }
        
        char8 *ptr = new char8[buf.length()];
        strcpy(ptr, buf.c_str());
        string s(ptr);
        delete[] ptr;
        return s;
        //printf("%s\n", ptr);
        return (const char8*)ptr;
    }
    string getContentFromPath(const string& filepath)
    {
        return getContentFromPath(filepath.c_str());
    }
    
    char8* LoadImage(const char8* filename, int32 *width, int32 *height)
    {
        const char8 *filepath = getPath(filename).c_str();
        CGDataProviderRef texturefiledata = CGDataProviderCreateWithFilename(filepath);
        
        if(!texturefiledata)
        {
            return NULL;
        }
        
        string extension;
        StringManipulator::GetExtensitonType(filename,extension);
        StringManipulator::ToLowerCase(extension);
        bool Ispng = StringManipulator::IsEqual(extension,"png") == 0;
        bool IsJpg = StringManipulator::IsEqual(extension,"jpg") == 0;
        
        CGImageRef textureImage;
        if(IsJpg)
            textureImage = CGImageCreateWithJPEGDataProvider(texturefiledata,
                                                             NULL,
                                                             true,
                                                             kCGRenderingIntentDefault);
        else if (Ispng)
            textureImage = CGImageCreateWithPNGDataProvider(texturefiledata,
                                                            NULL,
                                                            true,
                                                            kCGRenderingIntentDefault);
        else {
            cerr<<"LoadPng: unsupported image type:"<<extension<<"\n";
            return NULL;
        }
        
        CGDataProviderRelease(texturefiledata);
        *width = CGImageGetWidth(textureImage);
        *height = CGImageGetHeight(textureImage);
        void *imageData = malloc( *height * *width * 4 );
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGContextRef textureContext = CGBitmapContextCreate( imageData,
                                                            *width,
                                                            *height,
                                                            8, 4 * *width,
                                                            colorSpace,
                                                            kCGImageAlphaPremultipliedLast |
                                                            kCGBitmapByteOrder32Big );
        
        CGContextDrawImage( textureContext,
                           CGRectMake( 0,
                                      0,
                                      *width, 
                                      *height ), 
                           textureImage );
        
        CFRelease(textureContext);
        CGImageRelease(textureImage);
        
        return (char8*)imageData;
    }
    char8* LoadImage(const string& filename, int32 *width, int32 *height)
    {
        return LoadImage(filename.c_str(), width, height);
    }
    
    char8* LoadImageFromTga(const char8* filename, int32 *width, int32 *height)
    {
        return NULL;
    }
}
