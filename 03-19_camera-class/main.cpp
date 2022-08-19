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
#include "headers/Camera.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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

    camera = Camera();

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // get and handle user input events
        glfwPollEvents();

        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // bitwise OR - clear both color and depth buffer

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();

        // draw meshList[0]
        glm::mat4 model = glm::mat4(1.0f); // initialised to identity matrix
        
        // order of operations is important
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, 45.0f * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        meshList[0]->RenderMesh();

        // draw meshList[1]
        model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.5f));
        model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        meshList[0]->RenderMesh();

        glUseProgram(0); // unassigning the shader program

        mainWindow.swapBuffers();
    }

    exit(EXIT_SUCCESS);
}