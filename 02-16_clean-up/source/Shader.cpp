#include "../headers/Shader.h"

Shader::Shader()
{
    shaderId = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CreateFromString(const char *vertexCode, const char *fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode)
{
    // creating a shader program and returns its ID
    shaderId = glCreateProgram();

    if (!shaderId)
    {
        printf("Error creating shader program \n");
        return;
    }
    
    AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0; // result of the performed functions
    GLchar eLog[1024] = { 0 }; // place to log the errors

    glLinkProgram(shaderId); // linking the shader program
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result); // getting the result of the linking
    
    if (!result)
    {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog); // getting the error log
        printf("Error linking shaderId program: '%s' \n", eLog); // printing the error log
        return;
    }

    glValidateProgram(shaderId); // validating the shader program
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result); // getting the result of the validation
    
    if (!result)
    {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog); // getting the error log
        printf("Error validating shaderId program: '%s' \n", eLog); // printing the error log
        return;
    }

    // getting the location of the uniform variable
    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

void Shader::UseShader()
{
    glUseProgram(shaderId);
}

void Shader::ClearShader()
{
    if (shaderId != 0)
    {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);
    
    if (shaderObj == 0)
    {
        fprintf(stderr, "Error creating shader type %d \n", shaderType);
        exit(EXIT_FAILURE);
    }
    
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    glShaderSource(shaderObj, 1, theCode, codeLength);
    glCompileShader(shaderObj);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(shaderObj, 1024, NULL, eLog);
        fprintf(stderr, "Error compiling shader type %d: '%s' \n", shaderType, eLog);
        exit(EXIT_FAILURE);
    }
    
    glAttachShader(shaderProgram, shaderObj);
}

Shader::~Shader()
{
    ClearShader();
}