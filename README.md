# OpenGL Course

Udemy OpenGL course.

## Installation

### Ubuntu

GLEW : 
GLFW : 
GLM : `sudo apt install libglm-dev`

## Compiling

Line to compile from terminal on Linux: `g++ filename.cpp -o filename -lglfw3 -lGLEW -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl`

Line with less parameters that I also found to be working: `g++ main.cpp -o main -lglfw3 -lGLEW -lGL -lX11`

Compiling multiple files at once: `g++ main.cpp source/Mesh.cpp -o main -lglfw3 -lGLEW -lGL -lX11`

## Variable Qualifiers

Qualifiers give a special meaning to the variable. The following qualifiers are available:

* **const** – The declaration is of a compile time constant.
* **attribute** – Global variables that may change per vertex, that are passed from the OpenGL application to vertex shaders. This qualifier can only be used in vertex shaders. For the shader this is a read-only variable. See Attribute section.
* **uniform** – Global variables that may change per primitive [...], that are passed from the OpenGL application to the shaders. This qualifier can be used in both vertex and fragment shaders. For the shaders this is a read-only variable. See Uniform section.
* **varying** – used for interpolated data between a vertex shader and a fragment shader. Available for writing in the vertex shader, and read-only in a fragment shader. See Varying section.