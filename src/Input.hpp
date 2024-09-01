#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "ui.hpp"


void processInput(GLFWwindow *window);



// INPUT
struct InputStruct {
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

	
} input;





void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
        printf("Click! -- %f , %f \n", xpos, ypos);
		// printf("simstate: %d \n", SimState::idle);

        // if(simulation.simState == SimState::idle && ypos < 100.0 && xpos > 700.0){
		if(ypos < 100.0 && xpos > 700.0){
			printf("Start Simulation button clicked! \n");
            input.startSimClick = 1;
			// simulation.simState = SimState::startClickDetected;
		}
		input.mousePressActive = 1;
		input.pointerXLastClick = xpos;
		input.pointerYLastClick = ypos;
		
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		// printf("RELEASED LEFT MOUSE BUTON\n");
		input.mousePressActive = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS){
		glfwGetCursorPos(window, &xpos, &ypos);
		input.middleMouse = 1;
		input.pointerXLastFrame = xpos;
		input.pointerYLastFrame = ypos;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		input.middleMouse = 0;
	
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// printf("%f , %f \n", xpos, ypos);
	input.pointerX = xpos;
	input.pointerY = ypos;
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // printf("%d\n", key);
	// input.s = key;
	
	

	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL ) && action == GLFW_PRESS)
		input.ctrl = 1;
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)  && action == GLFW_RELEASE)
		input.ctrl = 0;
	

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		input.w = 1;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		input.s = 1;
	if (key == GLFW_KEY_A && action == GLFW_PRESS){
		input.a = 1;
		input.mostRecentADpress = 'a';
		// printf("A \n");
		// if(input.mostRecentADpress == 97)
		// 	printf("'a' is most recent\n");
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS){
		input.d = 1;
		input.mostRecentADpress = 'd';
		
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		input.w = 0;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		input.s = 0;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		input.a = 0;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		input.d = 0;

	// ARROW KEYS
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		input.au = 1;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		input.al = 1;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		input.ad = 1;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		input.ar = 1;

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		input.au = 0;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		input.al = 0;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		input.ad = 0;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		input.ar = 0;
}
 

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	// 	printf("S pressed! \n");
	
}


// void mouse_button_callback_2(GLFWwindow* window, int button, int action, int mods)
// {
//    printf("CALLBACK 2\n"); 
// }  