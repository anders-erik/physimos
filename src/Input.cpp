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

/** The raw pointer position object before sanity transformations.  */
PointerPosition pointer_pos_raw;
/** The authoritative pointer position object.  */
PointerPosition pointer_pos;
/** The authoritative pointer change object.  */
PointerChange pointer_change;

/** Authoritative key input modifier state.  */
ModifierState modifier_state;


// INPUT SUBSCRIBERS CALLBACK POINTERS
void (*conductor_callback_pointer_position)(PointerPosition _pointer_pos, PointerChange _pointer_change) = nullptr;
void (*conductor_callback_left_down)(PointerPosition _pointer_pos) = nullptr;
void (*conductor_callback_left_release)(PointerPosition _pointer_pos) = nullptr;
void (*conductor_callback_y_scroll)(double x_change) = nullptr;
void (*conductor_callback_key_up)(KeyEvent key_event) = nullptr;
void (*conductor_callback_key_down)(KeyEvent key_event) = nullptr;


// EXTERNAL 

void init() {
	physimos_window = get_initial_physimos_window();
	subscribeWindowChange_input(window_changed_callback);
}




void subscribe_pointer_position_conductor(void (*subscriberCallback)(PointerPosition _pointer_pos, PointerChange _pointer_change)) {
	conductor_callback_pointer_position = subscriberCallback;
}
void subscribe_mouse_left_down_conductor(void (*subscriberCallback)(PointerPosition _pointer_pos)) {
	conductor_callback_left_down = subscriberCallback;
}
void subscribe_mouse_left_release_conductor(void (*subscriberCallback)(PointerPosition _pointer_pos)) {
	conductor_callback_left_release = subscriberCallback;
}
void subscribe_mouse_scroll_y_conductor(void (*subscriberCallback)(double y_change)) {
	conductor_callback_y_scroll = subscriberCallback;
}
void subscribe_key_down_conductor(void (*subscriberCallback)(KeyEvent key_event)) {
	conductor_callback_key_down = subscriberCallback;
}
void subscribe_key_up_conductor(void (*subscriberCallback)(KeyEvent key_event)) {
	conductor_callback_key_up = subscriberCallback;
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	// if(ui_callback_y_scroll != nullptr)
	// 	ui_callback_y_scroll(yoffset);
	if(conductor_callback_y_scroll != nullptr)
		PInput::conductor_callback_y_scroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Callbacks
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// PInput::ui_callback_left_down(pointer_pos);
		PInput::conductor_callback_left_down(pointer_pos);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// ui_callback_left_release(pointer_pos);
		PInput::conductor_callback_left_release(pointer_pos);

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
	
		// GLFW_KEY_PAGE_UP
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
	// Compary raw value for easy difference calc. Measure from new position because inverted y axis
	pointer_change.dy = pointer_pos_raw.y - ypos;

	// Invert the read y value
	pointer_pos.x = xpos;
	pointer_pos.y = physimos_window.height / physimos_window.yscale - ypos;
	

	// printf("%f , %f \n", xpos, ypos);
	inputState.pointerX = xpos;
	inputState.pointerY = ypos;

	// PInput::pointerSubscriberCallback_ui(xpos, ypos);
	// ui_callback_pointer_position(pointer_pos, pointer_change);
	PInput::conductor_callback_pointer_position(pointer_pos, pointer_change);

	// save raw input
	pointer_pos_raw.x = xpos;
	pointer_pos_raw.y = ypos;
}

void window_changed_callback(PhysWin _physimos_window) {
	physimos_window.width = _physimos_window.width;
	physimos_window.height = _physimos_window.height;
	physimos_window.xscale = _physimos_window.xscale;
	physimos_window.yscale = _physimos_window.yscale;
}


PKey get_pkey(int key){
	switch (key)
	{
	case GLFW_KEY_B:
		return PKey::B;
		break;

	case GLFW_KEY_PAGE_UP:
		return PKey::PageUp;
		break;

	case GLFW_KEY_PAGE_DOWN:
		return PKey::PageDown;
		break;
	
	default:
		break;
	}

	return PKey::Unknown;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // printf("%d\n", key);
	// input.s = key;

	modifier_state.shift 	= mods & 1 ? true : false;
	modifier_state.ctrl 	= mods & 2 ? true : false;
	modifier_state.alt 		= mods & 4 ? true : false;
	
	// std::cout << "a"  << std::endl;

	if(action == GLFW_PRESS)
		conductor_callback_key_down({
			get_pkey(key),
			modifier_state,
			KeyDir::Down
		});

	if(action == GLFW_RELEASE)
		conductor_callback_key_up({
			get_pkey(key),
			modifier_state,
			KeyDir::Up
		});
	

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