
#include "Windowing.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.hpp"

#include "PSO_util.hpp"


GLFWwindow* window__;
int physimosLoadedOk;


PhysWin physimos_window;

void (*windowChangeCallback_ui)(PhysWin _physimos_window) = nullptr;
void (*windowChangeCallback_scene)(PhysWin _physimos_window) = nullptr;
void (*windowChangeCallback_input)(PhysWin _physimos_window) = nullptr;

PhysWin get_initial_physimos_window(){
    return physimos_window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    physimos_window.height = height;
    physimos_window.width = width;
    

    // Window property dependants
    if (windowChangeCallback_ui != nullptr)
        windowChangeCallback_ui(physimos_window);

    if (windowChangeCallback_scene != nullptr)
        windowChangeCallback_scene(physimos_window);
    
    if (windowChangeCallback_input != nullptr)
        windowChangeCallback_input(physimos_window);

}
void window_content_scale_callback(GLFWwindow* window, float xscale, float yscale)
{
    physimos_window.xscale = xscale;
    physimos_window.yscale = yscale;

    if (windowChangeCallback_ui != nullptr)
        windowChangeCallback_ui(physimos_window);
}


void subscribeWindowChange_ui(void (*subscriberCallback)(PhysWin physimos_window)) {
    windowChangeCallback_ui = subscriberCallback;
}
void subscribeWindowChange_scene(void (*subscriberCallback)(PhysWin physimos_window)) {
    windowChangeCallback_scene = subscriberCallback;
}
void subscribeWindowChange_input(void (*subscriberCallback)(PhysWin physimos_window)) {
    windowChangeCallback_input = subscriberCallback;
}



void initPhysimosWindow() {
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
        physimosLoadedOk = 0;
        return;
    }
    glfwMakeContextCurrent(window__);


    // SET PYSIMOS WINDOW OBJECT
    glfwGetWindowContentScale(window__, &physimos_window.xscale, &physimos_window.yscale);
    physimos_window.height = SCREEN_INIT_HEIGHT;
    physimos_window.width = SCREEN_INIT_WIDTH;

    /// WINDOW CALLBACKS
    glfwSetFramebufferSizeCallback(window__, framebuffer_size_callback);
    glfwSetWindowContentScaleCallback(window__, window_content_scale_callback);

    // INPUT DEVICE CALLBACKS
    glfwSetMouseButtonCallback(window__, PInput::mouse_button_callback);
    // glfwSetMouseButtonCallback(window__, mouse_button_callback_2); // this deactivates the first one!
    glfwSetCursorPosCallback(window__, PInput::cursor_position_callback);
    glfwSetKeyCallback(window__, PInput::key_callback);
    glfwSetScrollCallback(window__, PInput::scroll_callback);


    // glad: load all OpenGL function pointers  
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return -1;
        physimosLoadedOk = 0;
        return;
}

    physimosLoadedOk = 1;
}


int physimosWindowOK(){
    return physimosLoadedOk;
}
GLFWwindow* physimosReturnWindowPointer() {
    return window__;
}


// TERMINATE
int terminatePhysimosWindow() {
    glfwDestroyWindow(window__);
    glfwTerminate();
    return 0;
}



// Runtime
int windowIsStillGood(){
    return !glfwWindowShouldClose(window__);
}

void new_frame(){
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
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


























