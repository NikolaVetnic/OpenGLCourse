#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// window dimension 
const GLint WIDTH = 800;
const GLint HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

// translation parameters
bool direction = true; // right - true, left - false
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 5e-3f;

// rotation parameters
float currAngle = 0.0f;

// scaling parameters
bool sizeDirection = true;
float currSize = 0.5f;
float maxSize = 1.0f;
float minSize = 0.1f;

// vertex shader
static const char* vShader = "                                  \n\
    #version 330                                                \n\
                                                                \n\
    layout (location = 0) in vec3 pos;                          \n\
                                                                \n\
    out vec4 vCol;                                              \n\
                                                                \n\
    uniform mat4 model;                                         \n\
    uniform mat4 projection;                                    \n\
                                                                \n\
    void main()                                                 \n\
    {                                                           \n\
        gl_Position = projection * model * vec4(pos, 1.0f);     \n\
        vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);              \n\
    }                                                           \n\
";

// fragment shader
static const char* fShader = "                                  \n\
    #version 330                                                \n\
                                                                \n\
    in vec4 vCol;                                               \n\
                                                                \n\
    out vec4 color;                                             \n\
                                                                \n\
    void main()                                                 \n\
    {                                                           \n\
        color = vCol;                                           \n\
    }                                                           \n\
";

void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    // VAO holds multiple VBOs that define how a pyramid is drawn
    GLfloat vertices[] = {
        -1.0f * .67f, -1.0f * .33f,  0.0f,
         0.0f       ,  0.0f       , +1.0f,
        +1.0f * .67f, -1.0f * .33f,  0.0f,
         0.0f       , +1.0f * .67f,  0.0f
    };

    // creating a vertex array in the memory of GPU and returns its ID
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    // getting the location of the uniform variable
    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");
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

    glEnable(GL_DEPTH_TEST);

    // create viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateObjects();
    compileShaders();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(mainWindow))
    {
        // get and handle user input events
        glfwPollEvents();

        // translation parameters
        if (direction) // going right
            triOffset += triIncrement;
        else
            triOffset -= triIncrement;

        if (abs(triOffset) >= triMaxOffset)
            direction = !direction;

        // rotation parameters
        currAngle += 1.0f;

        if (currAngle >= 360.0f)
            currAngle -= 360.0f;

        // scaling parameters
        if (sizeDirection)
            currSize += 0.01f;
        else
            currSize -= 0.01f;

        if (currSize >= maxSize || currSize <= minSize)
            sizeDirection = !sizeDirection;

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // bitwise OR - clear both color and depth buffer

        glUseProgram(shader);

        glm::mat4 model = glm::mat4(1.0f); // initialised to identity matrix
        
        // order of operations is important
        model = glm::translate(model, glm::vec3(triOffset, triOffset, triOffset - 2.5f));
        model = glm::rotate(model, currAngle * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(VAO);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

                    // glDrawArrays(GL_TRIANGLES, 0, 3);
                    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glBindVertexArray(0); // unbind VAO

        glUseProgram(0); // unassigning the shader program

        glfwSwapBuffers(mainWindow);
    }

    exit(EXIT_SUCCESS);
}