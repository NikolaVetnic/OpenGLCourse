#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

// window dimension 
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shader, uniformXMove;

bool direction = true; // right - true, left - false
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 5e-3f;

// vertex shader
static const char* vShader = "                                  \n\
    #version 330                                                \n\
                                                                \n\
    layout (location = 0) in vec3 pos;                          \n\
                                                                \n\
    uniform float xMove;                                        \n\
                                                                \n\
    void main()                                                 \n\
    {                                                           \n\
        gl_Position = vec4(pos.x + xMove, pos.y, pos.z, 1.0f);  \n\
    }                                                           \n\
";

// fragment shader
static const char* fShader = "                                  \n\
    #version 330                                                \n\
                                                                \n\
    out vec4 color;                                             \n\
                                                                \n\
    void main()                                                 \n\
    {                                                           \n\
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);                   \n\
    }                                                           \n\
";

void CreateObjects()
{
    // VAO holds multiple VBOs that define how a triangle is drawn
    GLfloat vertices[] = {
        -0.5f, -0.5f,  0.0f,
        +0.5f, -0.5f,  0.0f,
         0.0f, +0.5f,  0.0f
    };

    // creating a vertex array in the memory of GPU and returns its ID
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void addShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType)
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

void compileShaders()
{
    // creating a shader program and returns its ID
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error creating shader program \n");
        return;
    }
    
    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0; // result of the performed functions
    GLchar eLog[1024] = { 0 }; // place to log the errors

    glLinkProgram(shader); // linking the shader program
    glGetProgramiv(shader, GL_LINK_STATUS, &result); // getting the result of the linking
    
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); // getting the error log
        printf("Error linking shader program: '%s' \n", eLog); // printing the error log
        return;
    }

    glValidateProgram(shader); // validating the shader program
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result); // getting the result of the validation
    
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); // getting the error log
        printf("Error validating shader program: '%s' \n", eLog); // printing the error log
        return;
    }

    uniformXMove = glGetUniformLocation(shader, "xMove"); // getting the location of the uniform variable
}

int main()
{
    if (!glfwInit())
    {
        printf("Error initialising GLFW \n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // allow forward compatibility

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

    if (!mainWindow)
    {
        printf("GLFW window creation failed \n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed \n");

        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateObjects();
    compileShaders();

    while (!glfwWindowShouldClose(mainWindow))
    {
        // get and handle user input events
        glfwPollEvents();

        if (direction) // going right
            triOffset += triIncrement;
        else
            triOffset -= triIncrement;

        if (abs(triOffset) >= triMaxOffset)
            direction = !direction;

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

            glUniform1f(uniformXMove, triOffset);
            glBindVertexArray(VAO);

                glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0); // unbind VAO

        glUseProgram(0); // unassigning the shader program

        glfwSwapBuffers(mainWindow);
    }

    exit(EXIT_SUCCESS);
}