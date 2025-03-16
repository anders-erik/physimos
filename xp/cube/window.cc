
#include "window.hh"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace xpcube {

GLFWwindow* window = NULL;

void framebuffer_callback(GLFWwindow* _window, int _width, int _height){
    glViewport(0, 0, _width, _height);
}



bool window_init(){


    glfwInit();
    window = glfwCreateWindow(600, 400, "HOLA", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, framebuffer_callback);

    return true;
}
void window_new_frame(){
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void window_end_of_frame(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}




InputState window_get_input(){

    InputState input_state;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        input_state.esc = true;


    return input_state;
}





void window_set_close(){
    glfwSetWindowShouldClose(window, true);
}
bool window_should_close(){
    return glfwWindowShouldClose(window);
}


}