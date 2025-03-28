
#include <iostream>

#include "auxwin.hh"

namespace window {

InputState input_state;

MouseState mouse_state;
GLFWcursor* default_cursor;
GLFWcursor* pan_cursor;

void framebuffer_callback(GLFWwindow* _window, int _width, int _height){
    glViewport(0, 0, _width, _height);
    // scene_set_viewport_dims(_width, _height);
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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
   

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        input_state.up = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        input_state.left = true;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        input_state.down = true;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        input_state.right = true;
       
}

void framebuffer_size_callback_2(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}

Auxwin::Auxwin(int width, int height){
    glfwInit();


	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 1);
    

    glfw_window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);

	// GLFWwindow *window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (glfw_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		// return -1;A
	}

    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_size_callback_2);

    // TODO: THIS SHOULD NOT BE FORCED WHEN CREATING A NEW WINDOW!
    glfwMakeContextCurrent(glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}


    glfwSetFramebufferSizeCallback(glfw_window, framebuffer_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, mouse_position_callback);
    glfwSetScrollCallback(glfw_window, scroll_callback);
    glfwSetKeyCallback(glfw_window, key_callback);

    glEnable(GL_MULTISAMPLE);
}


void Auxwin::make_current(){
    glfwMakeContextCurrent(glfw_window);
}
void Auxwin::close(){
    glfwSetWindowShouldClose(glfw_window, true);
    open = false;
}
bool Auxwin::is_open(){
    return !glfwWindowShouldClose(glfw_window);
    // return open;
}
void Auxwin::new_frame(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void Auxwin::end_frame(){
    glBindVertexArray(0); 

    glfwSwapBuffers(glfw_window);
	glfwPollEvents();
}


void Auxwin::destroy(){

    // TODO: SHOUDL NOT TERMINATE THE GLFW LIB!
    glfwTerminate();
}



InputState Auxwin::get_input_state(){
    // InputState input_state;

    // if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //     input_state.esc = true;
	// 	// auxwin.close();
    
    // return input_state;


    InputState _input_state = input_state;
    // Reset
    input_state = InputState();

    // Mouse
    _input_state.mouse = mouse_state;
    // reset
    mouse_state.middle_delta_accum.x = 0.0;
    mouse_state.middle_delta_accum.y = 0.0;


    _input_state.scroll_delta = mouse_state.scroll_accumulator;
    mouse_state.scroll_accumulator = 0.0f;

    if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        _input_state.esc = true;


    if (glfwGetKey(glfw_window, GLFW_KEY_P) == GLFW_PRESS)
        _input_state.p = true;


    if (glfwGetKey(glfw_window, GLFW_KEY_F) == GLFW_PRESS)
        _input_state.f = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_B) == GLFW_PRESS)
        _input_state.b = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS)
        _input_state.r = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_L) == GLFW_PRESS)
        _input_state.l = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_U) == GLFW_PRESS)
        _input_state.u = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
        _input_state.d = true;

    if(glfwGetKey(glfw_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(glfw_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        _input_state.shift = true;

    if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
        _input_state.s = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_M) == GLFW_PRESS)
        _input_state.m = true;


    if (glfwGetKey(glfw_window, GLFW_KEY_UP) == GLFW_PRESS)
        input_state.up = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        input_state.left = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        input_state.down = true;
    if (glfwGetKey(glfw_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        input_state.right = true;


    return _input_state;
}

}