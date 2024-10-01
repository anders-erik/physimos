#ifndef WINDOWING_HPP
#define WINDOWING_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>



// Init
void initGlThrowSimWindow();
int glThrowSimWindowOK();
GLFWwindow* glThrowSimReturnWindowPointer();

// Terminate
int terminatePhysimosWindow();

// Runtime
int shouldWindowKeepGoing();
void processInput();
void endOfFrameGlfwCalls();



#endif