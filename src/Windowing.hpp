#ifndef WINDOWING_HPP
#define WINDOWING_HPP

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>


struct GLFWwindow;

/** Holds window information. 
 *  Height & width are framebuffer size of window. 
 *  Scale value are OS configured values for scaling content on small, high resolution monitors, forcing me to scale certain content accordingly.
 * 
 *  NOTE: remember that the height and width represents actual framebuffer size which is passed to the shader -- viewport_height = height / yscale !
 */
typedef struct PhysWin {
    unsigned int height;
    unsigned int width;
    float xscale;
    float yscale;
} PhysWin;

/** Used by several physimos components to perform inital scaling. 
 *  Although continued querying will always return the most up to date window object, separate callbacks are used during runtime to dynamically notify dependants of changes.
 */
PhysWin get_initial_physimos_window();

// void subscribeFrameBufferUpdate(void (*subscriberCallback)(unsigned int height, unsigned int width));

/** Pass a callback function for window changes updates during runtime. NOTE: ONLY FOR ONE UI CALLBACK. */
void subscribeWindowChange_conductor(void (*subscriberCallback)(PhysWin physimos_window));
/** Pass a callback function for window changes updates during runtime.  NOTE: ONLY FOR ONE SCENE */
void subscribeWindowChange_scene(void (*subscriberCallback)(PhysWin physimos_window));
/** Pass a callback function for window changes updates during runtime.  NOTE: ONLY FOR ONE INPUT CALLBACK */
void subscribeWindowChange_input(void (*subscriberCallback)(PhysWin physimos_window));


/** Internal framebuffer callback */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


/** Create main physimos window. Specify logical size (pixel-dimensions / scale). */
void initPhysimosWindow(int _init_width, int _init_height);
int physimosWindowOK();
GLFWwindow* physimosReturnWindowPointer();

// Terminate
int terminatePhysimosWindow();

// Runtime
int windowIsStillGood();
void new_frame();
void processInput();
void endOfFrameGlfwCalls();



#endif