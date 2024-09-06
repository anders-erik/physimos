#include <iostream>

#include "Windowing.hpp"
#include "WorldScene.hpp"
#include "ui.hpp"
#include "Timing.hpp"





int main()
{
	std::cout << std::endl << "---------- MAIN() ----------" << std::endl << std::endl;
	srand(0);




	// glfw window creation
	// --------------------
	initGlThrowSimWindow();
	glThrowSimReturnWindowPointer();
	if (glThrowSimWindowOK() == 0) {
		return -1;
	}


	ws_init();



	ui_init();




	/* 
	
		MAIN LOOP
	
	*/

	while (shouldWindowKeepGoing())
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//
		// FPS INFO
		//
		timing_newFrame();



	
		// 
		// Check is escape is pressed
		// 
		processInput();



		ws_update();

		//
		// update and render ui
		//
		ui_update();
		


		//
		// Swap buffers
		//
		endOfFrameGlfwCalls();


		//
		// Force wait to limit FPS to specified constant
		//
		timing_waitForNextFrame();

	}


	// TODO: Delete all allocated opengl memory
	// glDeleteProgram(worldShader.ID);


	terminatGlThrowSimWindow();

	return 0;
}





