#include "../headers/Window.h"

Window::Window()
{
    width = 800;
    height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
}

int Window::Initialise()
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

    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

    if (!mainWindow)
    {
        printf("GLFW window creation failed \n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    GLenum error = glewInit();

	if (error != GLEW_OK)
    {
        printf("GLEW initialisation failed : %s \n", glewGetErrorString(glewInit()));

        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    // create viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    return 0;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}