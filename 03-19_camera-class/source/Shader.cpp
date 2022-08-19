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

void Shader::CreateFromFiles(const char *vertexLocation, const char *fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertexCode = vertexString.c_str(); // converts string to const char* array
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char *fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Failed to read %s \n", fileLocation);
        return "";
    }

    std::string line = "";

    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
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
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformView = glGetUniformLocation(shaderId, "view");
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint Shader::GetViewLocation()
{
    return uniformView;
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