#include <iostream>

#include "process_info.hpp"
#include "Windowing.hpp"
#include "WorldScene.hpp"
#include "ui.hpp"
#include "Timing.hpp"


#include "Simulator.hpp"

// TEMP FOR PNG DEV!
#include "png_loader.hpp"

int main()
{
	
	// std::cout << std::endl << "---------- MAIN() ----------" << std::endl << std::endl;
	
	// 2024-09-13
	std::cout << std::endl << "   Consistent incremental improvement is the fundamental action of anything worthwhile. " << std::endl;
	std::cout              << "   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ " << std::endl << std::endl;

	
	srand(0);


	// TEMP FOR PNG DEV!
	// png::loadFile("./media/4black.png");
	// png::prettyPrint();
	// return 0;


	// glfw window creation
	// --------------------
	initGlThrowSimWindow();
	glThrowSimReturnWindowPointer();
	if (glThrowSimWindowOK() == 0) {
		return -1;
	}


	ws_init();



	ui_init();


	process_info_init();



	// START SIMULATORS
	Sim::init();
	Sim::compute();


	/* 
	
		MAIN LOOP
	
	*/

	while (shouldWindowKeepGoing())
	{
		// process_info_init();

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





