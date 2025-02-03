#ifndef INPUT_HPP
#define INPUT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// #include "InputState.hpp"


// #include "Camera.hpp"

extern int inputX;


// INPUT
struct InputState {
	int startSimClick = 0;
	// char s = 0;
	int pointerX = 0;
	int pointerY = 0;
	int mousePressActive = 0;
	int pointerXLastClick = 0;
	int pointerYLastClick = 0;
	int pointerXLastFrame = 0;
	int pointerYLastFrame = 0;
	// keys
	int middleMouse = 0;
	int ctrl = 0;
	int w = 0;
	int a = 0;
	int s = 0;
	int d = 0;
	char mostRecentADpress = 's';
	int au = 0;
	int al = 0;
	int ad = 0;
	int ar = 0;

};

// InputState* getCurrentInputStatePointer();

// void setUiAndCamera(UI* _ui);





// void processInput(GLFWwindow *window);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


namespace Input {

void subscribeCursorPosition(void (*subscriberCallback)(double x, double y));
void subscribeLeftClickPosition(void (*subscriberCallback)(double x, double y));

}

#endif