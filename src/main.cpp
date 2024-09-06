
#include <iostream>
#include "Simulation.hpp"
#include "Camera.hpp"
#include "Windowing.hpp"
#include "WorldRenderer.hpp"
#include "Timing.hpp"
#include "ui.hpp"




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


	
	cam_init();
	
	wr_init();

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
		// SIMULATION
		//
		updateSimulation();
		

		// 
		// Check is escape is pressed
		// 
		processInput();


		//
		// Update Camera matrices (view/persp.)
		//
		cam_UpdateCam();



		//
		// Render all world objects (world renderer)
		//
		wr_render();
		

		//
		// update and render ui
		//
		ui_update();
		


		//
		// Swap buffers
		//
		endOfFrameGlfwCalls();


		//
		// Force wait to limit FPs to specified constant
		//
		timing_waitForNextFrame();

	}


	// TODO: Delete all allocated opengl memory
	// glDeleteProgram(worldShader.ID);


	terminatGlThrowSimWindow();

	return 0;
}





