#include "../headers/Window.h"

Window::Window()
{
    width = 800;
    height = 600;

    for (size_t i = 0; i < 1024; i++)
        keys[i] = 0;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    for (size_t i = 0; i < 1024; i++)
        keys[i] = 0;
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

    // handle key & mouse input
    createCallbacks();

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

    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            printf("Pressed : %d \n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            printf("Released : %d \n", key);
        }
    }
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}