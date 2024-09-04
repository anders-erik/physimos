

#include "Input.hpp"

int inputX = 1;



UI* ui_input;
// Camera* camera_input;

InputState inputState__;

InputState* getCurrentInputStatePointer() {
	return &inputState__;
}
void setUiAndCamera(UI* _ui) {
	ui_input = _ui;
}


// ----------------------------------------------------------------------------

// Keep pointers to main loop objects to enable direct change to input
// UI* ui_input;
// Camera* camera_input;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);


	// THESE ARE NOT REFERENCING THE PROPER INSTANCE OF CAMERA+UI ! ! ! !
	// camera_input->setPerspectiveMatrix(width, height);

	ui_input->setWindowSize(width, height);
	ui_input->reloadUi();
}
 
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// printf("MOUSE BUTTON CALLBACK!\n");
	double xpos, ypos;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
        printf("Click! -- %f , %f \n", xpos, ypos);
		// printf("simstate: %d \n", SimState::idle);

        // if(simulation.simState == SimState::idle && ypos < 100.0 && xpos > 700.0){
		if(ypos < 100.0 && xpos > 700.0){
			printf("Start Simulation button clicked! \n");
			inputState__.startSimClick = 1;
			// simulation.simState = SimState::startClickDetected;
		}
		inputState__.mousePressActive = 1;
		inputState__.pointerXLastClick = xpos;
		inputState__.pointerYLastClick = ypos;
		
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		// printf("RELEASED LEFT MOUSE BUTON\n");
		inputState__.mousePressActive = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
		inputState__.middleMouse = 1;
		inputState__.pointerXLastFrame = xpos;
		inputState__.pointerYLastFrame = ypos;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		inputState__.middleMouse = 0;
	
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// printf("%f , %f \n", xpos, ypos);
	inputState__.pointerX = xpos;
	inputState__.pointerY = ypos;
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // printf("%d\n", key);
	// input.s = key;
	
	

	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) && action == GLFW_PRESS)
		inputState__.ctrl = 1;
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)  && action == GLFW_RELEASE)
		inputState__.ctrl = 0;
	

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		inputState__.w = 1;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		inputState__.s = 1;
	if (key == GLFW_KEY_A && action == GLFW_PRESS){
		inputState__.a = 1;
		inputState__.mostRecentADpress = 'a';
		// printf("A \n");
		// if(inputState.mostRecentADpress == 97)
		// 	printf("'a' is most recent\n");
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS){
		inputState__.d = 1;
		inputState__.mostRecentADpress = 'd';
		
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		inputState__.w = 0;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		inputState__.s = 0;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		inputState__.a = 0;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		inputState__.d = 0;

	// ARROW KEYS
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		inputState__.au = 1;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		inputState__.al = 1;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		inputState__.ad = 1;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		inputState__.ar = 1;

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		inputState__.au = 0;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		inputState__.al = 0;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		inputState__.ad = 0;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		inputState__.ar = 0;
}
 

// // process all inputState: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// // ---------------------------------------------------------------------------------------------------------
// void processInput(GLFWwindow *window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);
// 	// if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
// 	// 	printf("S pressed! \n");
	
// }


// void mouse_button_callback_2(GLFWwindow* window, int button, int action, int mods)
// {
//    printf("CALLBACK 2\n"); 
// }  