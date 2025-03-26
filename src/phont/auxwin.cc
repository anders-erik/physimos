
#include <iostream>

#include "auxwin.hh"


void framebuffer_size_callback_2(GLFWwindow *window, int width, int height){
	glViewport(0, 0, width, height);
}

Auxwin::Auxwin(int width, int height){
    glfwInit();
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

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

InputState Auxwin::handle_input(){
    InputState input_state;

    if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        input_state.esc = true;
		// auxwin.close();
    
    return input_state;
}

void Auxwin::destroy(){

    // TODO: SHOUDL NOT TERMINATE THE GLFW LIB!
    glfwTerminate();
}
