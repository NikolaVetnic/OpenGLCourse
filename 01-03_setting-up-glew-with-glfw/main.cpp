#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main()
{
    // initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed \n");
        
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile == no backwards compatibility
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

    while (!glfwWindowShouldClose(mainWindow))
    {
        // get and handle user input events
        glfwPollEvents();

        // clear window
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    exit(EXIT_SUCCESS);
}