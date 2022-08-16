#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/Window.h" 
#include "headers/Mesh.h"
#include "headers/Shader.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

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

static const char* vShader = "Shaders/shader.vert"; // vertex shader
static const char* fShader = "Shaders/shader.frag"; // fragment shader

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

    Mesh* obj0 = new Mesh();

    obj0->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj0); // add to the end of list of meshes
}

void CreateShaders()
{
    Shader *shader0 = new Shader();

    shader0->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader0);
}

int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getShouldClose())
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

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();

        // draw meshList[0]
        glm::mat4 model = glm::mat4(1.0f); // initialised to identity matrix
        
        // order of operations is important
        model = glm::translate(model, glm::vec3(triOffset, triOffset, triOffset - 2.5f));
        model = glm::rotate(model, currAngle * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        meshList[0]->RenderMesh();

        // draw meshList[1]
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(-triOffset, -triOffset + .75f, -triOffset - 2.5f));
        model = glm::rotate(model, currAngle * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        meshList[0]->RenderMesh();

        glUseProgram(0); // unassigning the shader program

        mainWindow.swapBuffers();
    }

    exit(EXIT_SUCCESS);
}