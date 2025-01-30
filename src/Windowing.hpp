#ifndef WINDOWING_HPP
#define WINDOWING_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>



// Init
void initPhysimosWindow();
int physimosWindowOK();
GLFWwindow* physimosReturnWindowPointer();

// Terminate
int terminatePhysimosWindow();

// Runtime
int windowIsStillGood();
void processInput();
void endOfFrameGlfwCalls();



#endif