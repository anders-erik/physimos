#include <iostream>

#include "window.hh"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "scene.hh"

namespace xprubik {

GLFWwindow* window = NULL;

MouseState mouse_state;
GLFWcursor* default_cursor;
GLFWcursor* pan_cursor;

void framebuffer_callback(GLFWwindow* _window, int _width, int _height){
    glViewport(0, 0, _width, _height);
    scene_set_viewport_dims(_width, _height);
}


void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    // std::cout << "mouse butn" << std::endl;
    if(button == GLFW_MOUSE_BUTTON_MIDDLE){
        if(action == GLFW_PRESS){
            // std::cout << "moddle press" << std::endl;
            mouse_state.middle_down = true;
            
            glfwSetCursor(window, pan_cursor);
        }
        else if (action == GLFW_RELEASE){
            // std::cout << "moddle rel" << std::endl;
            mouse_state.middle_down = false;
            glfwSetCursor(window, default_cursor);
        }

        mouse_state.middle_delta_accum.x = 0.0;
        mouse_state.middle_delta_accum.y = 0.0;
        mouse_state.middle_prev_pos.x = mouse_state.cursor_pos.x;
        mouse_state.middle_prev_pos.y = mouse_state.cursor_pos.y;
    }
    
}

void mouse_position_callback(GLFWwindow *window, double xpos, double ypos){

    // std::cout << "mouse mouse" << std::endl;
    // std::cout << "accum x = " << mouse_state.middle_delta_accum.x << std::endl;
    // std::cout << "accum y = " << mouse_state.middle_delta_accum.y << std::endl;

    if(mouse_state.middle_down){
        mouse_state.middle_delta_accum.x += xpos - mouse_state.middle_prev_pos.x;
        mouse_state.middle_delta_accum.y += mouse_state.middle_prev_pos.y - ypos;

        mouse_state.middle_prev_pos.x = xpos;
        mouse_state.middle_prev_pos.y = ypos;
    
    }

    mouse_state.cursor_pos.x = xpos;
    mouse_state.cursor_pos.y = ypos;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    mouse_state.scroll_accumulator += (float) yoffset;
    // std::cout << "mouse_state.scroll_accumulator = " << mouse_state.scroll_accumulator << std::endl;
}



bool window_init(){


    glfwInit();
    window = glfwCreateWindow(600, 400, "HOLA", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, framebuffer_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    default_cursor = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
    pan_cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);

    return true;
}
void window_new_frame(){
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    
}
void window_end_of_frame(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}




InputState window_get_input(){

    InputState input_state;

    // Mouse
    input_state.mouse = mouse_state;
    // reset
    mouse_state.middle_delta_accum.x = 0.0;
    mouse_state.middle_delta_accum.y = 0.0;


    input_state.scroll_delta = mouse_state.scroll_accumulator;
    mouse_state.scroll_accumulator = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        input_state.esc = true;


    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        input_state.p = true;


    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        input_state.f = true;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        input_state.b = true;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        input_state.r = true;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        input_state.l = true;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        input_state.u = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        input_state.d = true;

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        input_state.shift = true;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        input_state.up = true;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        input_state.left = true;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        input_state.down = true;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        input_state.right = true;


    return input_state;
}





void window_set_close(){
    glfwSetWindowShouldClose(window, true);
}
bool window_should_close(){
    return glfwWindowShouldClose(window);
}


}