
#include "Windowing.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Input.hpp"

#include "PSO_util.hpp"


GLFWwindow* window__;
int glThrowSimLoadedOk;





void initGlThrowSimWindow() {
    glfwInit();
    // glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window__ = glfwCreateWindow(SCREEN_INIT_WIDTH, SCREEN_INIT_HEIGHT, "Physimos", NULL, NULL);

    if (window__ == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // return -1;
        glThrowSimLoadedOk = 0;
        return;
    }
    glfwMakeContextCurrent(window__);

    // User input
    glfwSetFramebufferSizeCallback(window__, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window__, mouse_button_callback);
    // glfwSetMouseButtonCallback(window__, mouse_button_callback_2); // this deactivates the first one!
    glfwSetCursorPosCallback(window__, cursor_position_callback);
    glfwSetKeyCallback(window__, key_callback);



    // glad: load all OpenGL function pointers  
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return -1;
        glThrowSimLoadedOk = 0;
        return;
}

    glThrowSimLoadedOk = 1;
}


int glThrowSimWindowOK(){
    return glThrowSimLoadedOk;
}
GLFWwindow* glThrowSimReturnWindowPointer() {
    return window__;
}


// TERMINATE
int terminatePhysimosWindow() {
    glfwDestroyWindow(window__);
    glfwTerminate();
    return 0;
}



// Runtime
int shouldWindowKeepGoing(){
    return !glfwWindowShouldClose(window__);
}

// process all inputState: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput()
{
    if (glfwGetKey(window__, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window__, true);
    // if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    // 	printf("S pressed! \n");

}

// End of frame calls
void endOfFrameGlfwCalls(){

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window__);
    glfwPollEvents();

}


























