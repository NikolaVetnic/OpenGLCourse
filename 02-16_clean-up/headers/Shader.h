#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>

class Shader
{
    public:
        Shader();

        void CreateFromString(const char *vertexCode, const char *fragmentCode);
        void CreateFromFiles(const char *vertexLocation, const char *fragmentLocation);

        std::string ReadFile(const char *fileLocation);

        GLuint GetProjectionLocation();
        GLuint GetModelLocation();

        void UseShader();
        void ClearShader();

        ~Shader();

    private:
        GLuint shaderId, uniformProjection, uniformModel;

        void CompileShader(const char *vertexCode, const char *fragmentCode);
        void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType);
};