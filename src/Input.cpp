

#include "Input.hpp"

// #include "ui_old.hpp"
#include "scene/Camera.hpp"
#include "scene/pscene.hh"
#include "ui/ui.hh"

int inputX = 1;



// UI* ui_input;
// Camera* camera_input;

// InputState inputState__;




// InputState* getCurrentInputStatePointer() {
// 	return &inputState__;
// }
// void setUiAndCamera(UI* _ui) {
// 	ui_input = _ui;
// }


// ----------------------------------------------------------------------------

// Keep pointers to main loop objects to enable direct change to input
// UI* ui_input;
// Camera* camera_input;






namespace PInput {

	InputState inputState;

	// Only one subscriber implemented
	void (*cursorSubscriberCallback)(double x, double y) = nullptr;
	void (*leftClickSubscriberCallback)(double x, double y) = nullptr;

	void subscribeCursorPosition(void (*subscriberCallback)(double x, double y)) {
		cursorSubscriberCallback = subscriberCallback;
	}

	void subscribeLeftClickPosition(void (*subscriberCallback)(double x, double y)) {
		leftClickSubscriberCallback = subscriberCallback;
	}

	




void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Callbacks
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		PInput::leftClickSubscriberCallback(xpos, ypos);
	}
	
	// printf("MOUSE BUTTON CALLBACK!\n");
	
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		
        // printf("Click! -- %f , %f \n", xpos, ypos);
		// printf("simstate: %d \n", SimState::idle);

        // if(simulation.simState == SimState::idle && ypos < 100.0 && xpos > 700.0){
		// if(ypos < 100.0 && xpos > 700.0){
		// 	printf("Start Simulation button clicked! \n");
		// 	inputState.startSimClick = 1;
		// 	// simulation.simState = SimState::startClickDetected;
		// }
		inputState.mousePressActive = 1;
		inputState.pointerXLastClick = xpos;
		inputState.pointerYLastClick = ypos;
		
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		// printf("RELEASED LEFT MOUSE BUTON\n");
		inputState.mousePressActive = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
		inputState.middleMouse = 1;
		inputState.pointerXLastFrame = xpos;
		inputState.pointerYLastFrame = ypos;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		inputState.middleMouse = 0;
	

	// NOTIFY UI OF BUTTON CLICK
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
		
		// OLD UI
		// ui_detectElementClick(xpos, ypos);
	}

}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// printf("%f , %f \n", xpos, ypos);
	inputState.pointerX = xpos;
	inputState.pointerY = ypos;

	PInput::cursorSubscriberCallback(xpos, ypos);
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // printf("%d\n", key);
	// input.s = key;
	
	

	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) && action == GLFW_PRESS)
		inputState.ctrl = 1;
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)  && action == GLFW_RELEASE)
		inputState.ctrl = 0;
	

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		inputState.w = 1;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		inputState.s = 1;
	if (key == GLFW_KEY_A && action == GLFW_PRESS){
		inputState.a = 1;
		inputState.mostRecentADpress = 'a';
		// printf("A \n");
		// if(inputState.mostRecentADpress == 97)
		// 	printf("'a' is most recent\n");
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS){
		inputState.d = 1;
		inputState.mostRecentADpress = 'd';
		
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		inputState.w = 0;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		inputState.s = 0;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		inputState.a = 0;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		inputState.d = 0;

	// ARROW KEYS
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		inputState.au = 1;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		inputState.al = 1;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		inputState.ad = 1;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		inputState.ar = 1;

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		inputState.au = 0;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		inputState.al = 0;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		inputState.ad = 0;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		inputState.ar = 0;
}
 
} // End INPUT