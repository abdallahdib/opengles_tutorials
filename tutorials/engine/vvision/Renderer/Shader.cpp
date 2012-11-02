/*
 *  Shader.cpp
 *  Virtual Vision
 *
 *  Created by Abdallah Dib Abdallah.dib@virtual-vison.net
 *  Copyright 2011 Virtual Vision. All rights reserved.
 *
 */

#include "Shader.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
namespace vvision
{
    
    CShader::CShader()
    {
        
        m_uShadersProgram = glCreateProgram();
        
    }
    CShader::~CShader()
    {
        if (m_uVertShader)
            glDeleteShader(m_uVertShader);
        
        if (m_uFragShader)
            glDeleteShader(m_uFragShader);
        
        if (m_uShadersProgram)
        {
            glDeleteProgram(m_uShadersProgram);
            CShader::m_uShadersProgram = 0;
        }
    }
    
    bool CShader::LoadShadersFromMemory(const char8 *vertex_shader, const char8* fragment_shader)
    {
        if(!m_uShadersProgram)
            m_uShadersProgram = glCreateProgram();
        
        if ( !CompileShader(&m_uVertShader, GL_VERTEX_SHADER, vertex_shader) )
            return false;
        
        if ( !CompileShader(&m_uFragShader, GL_FRAGMENT_SHADER, fragment_shader) )
            return false;
        
        // Attach vertex shader to ShadersProgram.
        glAttachShader(m_uShadersProgram, m_uVertShader);
        
        // Attach fragment shader to ShadersProgram.
        glAttachShader(m_uShadersProgram, m_uFragShader);
        
        // Bind attribute locations.
        // This needs to be done prior to linking.
        glBindAttribLocation(m_uShadersProgram, ATTRIB_VERTEX, "position");
        glBindAttribLocation(m_uShadersProgram, ATTRIB_NORMAL, "normal");
        glBindAttribLocation(m_uShadersProgram, ATTRIB_COLOR, "color");
        glBindAttribLocation(m_uShadersProgram, ATTRIB_TANGENT, "tangent");
        glBindAttribLocation(m_uShadersProgram, ATTRIB_TEXTURE0, "texCoord0");
        
        glBindAttribLocation(m_uShadersProgram, ATTRIB_BONES_INDICES, "bones");
        glBindAttribLocation(m_uShadersProgram, ATTRIB_BONES_WEIGHTS, "weights");
        
        if (!LinkProgram())
        {
            //print log here
            
            if (m_uVertShader)
            {
                glDeleteShader(m_uVertShader);
                m_uVertShader = 0;
            }
            
            if (m_uFragShader)
            {
                glDeleteShader(m_uFragShader);
                m_uFragShader = 0;
            }
            
            return false;
        }
        
        LoadShaderUniforms();
        return true;
    }
    bool CShader::CompileShader(GLuint *shader, GLenum type, const char8* file )
    {
        GLint status;
        const GLchar *source = (GLchar*) file;
        
        if (!source || !strlen(source))
            return false;
        
        *shader = glCreateShader(type);
        glShaderSource(*shader, 1, &source, NULL);
        glCompileShader(*shader);
        
        
        GLint logLength;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        
#ifdef DEBUG
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(*shader, logLength, &logLength, log);
            
            // print log  here
            cerr<<"CShader: [INFO] shader compile log: "<<log<<endl;
            
            free(log);
        }
#endif
        
        glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
        
        if (status == 0)
        {
#ifdef DEBUG
            cerr<<"CShader: [ERROR] failed to compile shader."<<endl;
#endif
            glDeleteShader(*shader);
            return false;
        }
        
        return true;
        
    }
    bool CShader::LinkProgram()
    {
        GLint status;
        glLinkProgram(m_uShadersProgram);
        
#ifdef DEBUG_SHADER_LINK_OUTPUT
        GLint logLength;
        glGetProgramiv(m_uShadersProgram, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetProgramInfoLog(m_uShadersProgram, logLength, &logLength, log);
            // print log  here
            cerr<<"CShader: [INFO] link output:\n"<< log<<endl;
            free(log);
        }
#endif
        
        glGetProgramiv(m_uShadersProgram, GL_LINK_STATUS, &status);
        if (m_uShadersProgram == 0)
            return false;
        
        return true;
        
    }
    bool CShader::ValidateProgram()
    {
        GLint logLength, status;
        
        glValidateProgram(m_uShadersProgram);
        glGetProgramiv(m_uShadersProgram, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetProgramInfoLog(m_uShadersProgram, logLength, &logLength, log);
            // print log  here
            free(log);
        }
        
        glGetProgramiv(m_uShadersProgram, GL_VALIDATE_STATUS, &status);
        if (status == 0)
            return false;
        
        return true;
        
    }
    bool CShader::SetUniform1f(const GLchar* uniformName, GLfloat value,GLint location)
    {
        if (location == -1) {
            int loc =glGetUniformLocation(m_uShadersProgram, uniformName);
            if (loc != -1)
            {
                glUniform1f(loc, value);
                return true;
            }
            else
                return false;
            
        }
        else {
            //if u are here that mean that u have already  verified  that the location alread exist using getuniformlocation, so be careful
            glUniform1f(location, value);
            return true;
        }
        
    }
    bool CShader::SetUniform1i(const GLchar* uniformName, GLint value, GLint location )
    {
        if (location == -1) {
            int loc =glGetUniformLocation(m_uShadersProgram, uniformName);
            
            if (loc != -1)
            {
                glUniform1i(loc, value);
                return true;
            }
            else
                return false;
            
        }
        else {
            //if u are here that mean that u have already  verified  that the location alread exist using getuniformlocation, so be careful
            glUniform1i(location, value);
            
            return true;
        }
    }
    bool CShader::SetUniform2fv(const GLchar* uniformName, GLsizei count, const GLfloat* value, GLint location)
    {
        if (location == -1) {
            int loc =glGetUniformLocation(m_uShadersProgram, uniformName);
            if (loc != -1)
            {
                glUniform2fv(loc, count, value);
                return true;
            }
            else
                return false;
            
        }
        else {
            //if u are here that mean that u have already  verified  that the location alread exist using getuniformlocation, so be careful
            glUniform2fv(location, count, value);
            return true;
        }
    }
    bool CShader::SetUniform3fv(const GLchar* uniformName, GLsizei count, const GLfloat* value, GLint location)
    {
        if (location == -1) {
            int loc =glGetUniformLocation(m_uShadersProgram, uniformName);
            if (loc != -1)
            {
                glUniform3fv(loc, count, value);
                return true;
            }
            else
                return false;
            
        }
        else {
            //if u are here that mean that u have already  verified  that the location alread exist using getuniformlocation, so be careful
            glUniform3fv(location, count, value);
            return true;
        }
    }
    
    bool CShader::SetUniform4fv(const GLchar* uniformName, GLsizei count, const GLfloat* value, GLint location)
    {
        if (location == -1) {
            int loc =glGetUniformLocation(m_uShadersProgram, uniformName);
            if (loc != -1)
            {
                glUniform4fv(loc, count, value);
                return true;
            }
            else
                return false;
            
        }
        else {
            //if u are here that mean that u have already  verified  that the location alread exist using getuniformlocation, so be careful
            glUniform4fv(location, count, value);
            return true;
        }
    }
    
    bool CShader::SetUniformMatrix4x4fv(const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value,int location)
    {
        if (location == -1) {
            int loc =glGetUniformLocation(m_uShadersProgram, uniformName);
            if (loc != -1)
            {
                glUniformMatrix4fv(loc, count, transpose, value);
                return true;
            }
            else
                return false;
        }
        else {
            glUniformMatrix4fv(location, count, transpose, value);
            return true;
        }
    }
    bool CShader::SetUniformMatrix3x3fv(const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value, int location)
    {
        if (location == -1) {
            int loc =glGetUniformLocation(m_uShadersProgram, uniformName);
            if (loc != -1)
            {
                glUniformMatrix3fv(loc, count, transpose, value);
                return true;
            }
            else
                return false;
        }
        else {
            glUniformMatrix3fv(location, count, transpose, value);
            return true;
        }
    }
    GLint CShader::GetUnifomLocation(const GLchar* uniformName)
    {
        return glGetUniformLocation(m_uShadersProgram, uniformName);
    }
    void CShader::Begin()
    {
        if (m_uShadersProgram <= 0)
            return;
        
        glUseProgram(m_uShadersProgram);
    }
    void CShader::End()
    {
        glUseProgram(0);
    }
    void CShader::LoadShaderUniforms()
    {
        //matrices
        matproj = glGetUniformLocation(m_uShadersProgram,               "matProj");
        matview = glGetUniformLocation(m_uShadersProgram,               "matView");
        matmodel = glGetUniformLocation(m_uShadersProgram,              "matModel");
        matViewModel  = glGetUniformLocation(m_uShadersProgram,          "matViewModel");
        matprojviewmodel = glGetUniformLocation(m_uShadersProgram,      "matProjViewModel");
        matnormal = glGetUniformLocation(m_uShadersProgram,             "matNormal");
        
        //attribute
        position = glGetAttribLocation(m_uShadersProgram,   "position");
        normal = glGetAttribLocation(m_uShadersProgram,     "normal");
        texcoord0 = glGetAttribLocation(m_uShadersProgram,  "texCoord0");
        color = glGetAttribLocation(m_uShadersProgram,      "color");
        tangent = glGetAttribLocation(m_uShadersProgram,    "tangent");
        
        ///diffuse
        texture0 = glGetUniformLocation(m_uShadersProgram, "texture0");
        
        //cube map
        textureCubeMap = glGetUniformLocation(m_uShadersProgram, "textureCubeMap");
        
        //bump
        textureBump = glGetUniformLocation(m_uShadersProgram, "textureBump");
        
        //specular
        textureSpecular = glGetUniformLocation(m_uShadersProgram, "textureSpecular");
        //detail
        textureDetail = glGetUniformLocation(m_uShadersProgram,     "textureDetail");
        useDetailTexture = glGetUniformLocation(m_uShadersProgram,  "useDetailTexture");
        detailFactor = glGetUniformLocation(m_uShadersProgram,      "detailFactor");
        
        //material
        matColorAmbient = glGetUniformLocation(m_uShadersProgram,   "matColorAmbient");
        matColorDiffuse = glGetUniformLocation(m_uShadersProgram,   "matColorDiffuse");
        matColorSpecular = glGetUniformLocation(m_uShadersProgram,  "matColorSpecular");
        matShininess = glGetUniformLocation(m_uShadersProgram,      "matShininess");
        matOpacity  = glGetUniformLocation(m_uShadersProgram,       "matOpacity");
        matColorEmissive = glGetUniformLocation(m_uShadersProgram,  "matColorEmissive");
        
        //fog
        fcolor = glGetUniformLocation(m_uShadersProgram,    "fColor");
        fstartend = glGetUniformLocation(m_uShadersProgram, "fStartEnd");
        
        //light
        lightColorAmbient = glGetUniformLocation(m_uShadersProgram,     "lightColorAmbient");
        lightColorDiffuse = glGetUniformLocation(m_uShadersProgram,     "lightColorDiffuse");
        lightColorSpecular = glGetUniformLocation(m_uShadersProgram,    "lightColorSpecular");
        lightPosition = glGetUniformLocation(m_uShadersProgram,         "lightPosition");
        lightAttenuation = glGetUniformLocation(m_uShadersProgram,      "lightAttenuation");
        
        //time
        time = glGetUniformLocation(m_uShadersProgram, "Time");
        
        //position
        cameraPosition = glGetUniformLocation(m_uShadersProgram, "cameraPosition");
        
    }
}
