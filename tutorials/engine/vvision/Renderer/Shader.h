/* Shader.h
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

#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "types.h"

namespace vvision
{
    class CShader
    {
        
        public :
        
        /** constructor*/
        CShader();
        
        /** destructor*/
        ~CShader();
        
        /** load compile and link shader, return true if success
         * @param vetex_shader  this is a string containing the shader source file (  not the file name of the shader but the content of the file !!!!)
         * @param fragment_shader same ...
         */
        bool LoadShadersFromMemory(const char8 *vertex_shader, const char8* fragment_shader);
        
        /** send float unifrom to shader
         * set location = -1 if you dont know the location of the uniform
         * @return true if success
         */
        bool SetUniform1f(const GLchar* uniformName, GLfloat value, GLint location = -1);
        
        /** send int uniform to shader*/
        bool SetUniform1i(const GLchar* uniformName, GLint value, GLint location = -1);
        
        /** send 2d flaot vector to shader*/
        bool SetUniform2fv(const GLchar* uniformName, GLsizei count, const GLfloat* value, GLint location = -1);
        
        /** send 3d float vector to shader*/
        bool SetUniform3fv(const GLchar* uniformName, GLsizei count, const GLfloat* value, GLint location = -1);
        
        /** send 4d float vector to shader*/
        bool SetUniform4fv(const GLchar* uniformName, GLsizei count, const GLfloat* value, GLint location = -1);
        
        
        /** send 4*4 vector to shader*/
        bool SetUniformMatrix4x4fv(const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value, int32 location = -1);
        
        /** send 3x3 matrix to shader*/
        bool SetUniformMatrix3x3fv(const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value, int32 location = -1);
        
        /** get the uniform location given unifrom name*/
        GLint GetUnifomLocation(const GLchar* uniformName);
        
        /** validate shader ( used in debug mode)
         * check if shader is setup correctly
         */
        bool ValidateProgram();
        
        /** activate shader*/
        void Begin();
        
        /** desactivate shader*/
        void End();
        
        /** unifroms*/
        int32 matproj;
        int32 matview;
        int32 matmodel;
        int32 matViewModel;
        int32 matnormal;
        int32 matprojviewmodel;
        
        /** diffuse texture*/
        int32 texture0;
        
        /** texture cubemap*/
        int32 textureCubeMap;
        
        /** texture bump*/
        int32 textureBump;
        
        /**texture specular*/
        int32 textureSpecular;
        
        /** texture detail*/
        int32 textureDetail;
        
        /** is detail texture enabled*/
        int32 useDetailTexture;
        
        /** detail factor */
        int32 detailFactor;
        
        /** attributes*/
        int32 position;
        int32 normal;
        int32 texcoord0;
        int32 color;
        int32 tangent;
        
        /** material uniforms*/
        int32 matShininess;
        int32 matOpacity;
        int32 matColorSpecular;
        int32 matColorDiffuse;
        int32 matColorAmbient;
        int32 matColorEmissive;
        
        /** fog attribute*/
        int32 fcolor;
        int32 fstartend;
        
        /** light uniforms*/
        int32 lightColorAmbient;
        int32 lightColorDiffuse;
        int32 lightColorSpecular;
        int32 lightPosition;
        int32 lightAttenuation;
        
        /** time */
        int32 time;
        
        /** camera*/
        int32 cameraPosition;
        
        private :
        
        /** not allowed*/
        CShader(const CShader& shader);
        CShader& operator=(const CShader& shader);
        
        /** compile shader*/
        bool CompileShader(GLuint *shader, GLenum type, const char8* file );
        
        /** link*/
        bool LinkProgram();
        
        /** uniform locations*/
        void LoadShaderUniforms();
        
        /** shader program*/
        GLuint m_uShadersProgram;
        
        /** vertex shader*/
        GLuint m_uVertShader;
        
        /** fragment shader*/
        GLuint m_uFragShader;
        
    };
}
#endif