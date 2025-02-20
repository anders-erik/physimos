#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <stdio.h>

#include "Input.hpp"

// #include "ui_old.hpp"
#include "scene/Camera.hpp"
#include "scene/pscene.hh"
#include "ui/ui.hh"





namespace PInput {

// INTERNAL

/** Callback function hat the window will call to update the hpysimos window object in the PInput namespace */
void window_changed_callback(PhysWin _physimos_window);


InputState inputState;


/** The physimos window object in the input namespace.  */
PhysWin physimos_window;

/** The authoritative pointer position object.  */
PointerPosition pointer_pos;
/** The authoritative pointer change object.  */
PointerChange pointer_change;



// INPUT SUBSCRIBERS CALLBACK POINTERS
void (*pointerSubscriberCallback_ui)(PointerPosition _pointer_pos, PointerChange _pointer_change) = nullptr;
void (*leftClickSubscriberCallback_ui)(PointerPosition _pointer_pos) = nullptr;
void (*leftReleaseSubscriberCallback_ui)(PointerPosition _pointer_pos) = nullptr;
void (*callback_y_scroll_ui)(double x_change) = nullptr;


// EXTERNAL 

void init() {
	physimos_window = get_initial_physimos_window();
	subscribeWindowChange_input(window_changed_callback);
}



void subscribePointerPosition_ui(void (*subscriberCallback)(PointerPosition _pointer_pos, PointerChange _pointer_change)) {
	pointerSubscriberCallback_ui = subscriberCallback;
}

void subscribeMouseLeftClick_ui(void (*subscriberCallback)(PointerPosition _pointer_pos)) {
	leftClickSubscriberCallback_ui = subscriberCallback;
}
void subscribeMouseLeftRelease_ui(void (*subscriberCallback)(PointerPosition _pointer_pos)) {
	leftReleaseSubscriberCallback_ui = subscriberCallback;
}
void subscribeMouseScrollY_ui(void (*subscriberCallback)(double y_change)) {
	callback_y_scroll_ui = subscriberCallback;
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	if(callback_y_scroll_ui != nullptr)
		callback_y_scroll_ui(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Callbacks
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		PInput::leftClickSubscriberCallback_ui(pointer_pos);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		leftReleaseSubscriberCallback_ui(pointer_pos);
		inputState.mousePressActive = 0;
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
	// New value measured from previous update value
	pointer_change.dx = xpos - pointer_pos.x;
	pointer_change.dy = ypos - pointer_pos.y;
	// pointer_pos.dx = xpos - pointer_pos.x;
	// pointer_pos.dy = ypos - pointer_pos.y;

	// Invert the read y value
	pointer_pos.x = xpos;
	pointer_pos.y = physimos_window.height - ypos;
	

	// printf("%f , %f \n", xpos, ypos);
	inputState.pointerX = xpos;
	inputState.pointerY = ypos;

	// PInput::pointerSubscriberCallback_ui(xpos, ypos);
	pointerSubscriberCallback_ui(pointer_pos, pointer_change);
}

void window_changed_callback(PhysWin _physimos_window) {
	physimos_window.width = _physimos_window.width;
	physimos_window.height = _physimos_window.height;
	physimos_window.xscale = _physimos_window.xscale;
	physimos_window.yscale = _physimos_window.yscale;
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