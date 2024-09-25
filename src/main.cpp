#include <iostream>

#include "process_info.hpp"
#include "Windowing.hpp"
#include "WorldScene.hpp"
#include "ui.hpp"
#include "Timing.hpp"

#include "Math.hpp"


#include "Simulator.hpp"

// TEMP FOR PNG DEV!
#include "png_loader.hpp"

int main()
{
	// math::pvec3 a = { 1.0, 2.0, 3.0 };
	// math::pvec3 b = { 4.0, 5.0, 6.0 };
	// math::add_v3(a, b);

	// math::pvec3 crossOutput = {0};
	// math::cross_v3(a, b, crossOutput);

	// // std::cout << "In main: " << a[0] << std::endl;

	// math::print_v3(crossOutput);

	// std::cout << "OKOKOK"  << std::endl;
	
	// return 0;
	
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



	// START SIMULATORS
	// Make sure we initialize the simulator BEFORE grabbing it when setting up Simulator continer in the world scene
	Sim::init();
	Sim::compute();


	ws_init();



	ui_init();


	process_info_init();



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





