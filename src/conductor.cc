
#include "Windowing.hpp"
#include "shader.hpp"
#include "scene/pscene.hh"
#include "ui/ui.hh"
#include "Timing.hpp"

#include "Input.hpp"

#include "Simulator.hpp"

#include "resources.hh"

#include "lib/lib.hh"

#include "conductor.hh"



int conductor_rouse()
{
	
	// START LOADING RESOURCES - ATM ONLY GETS MODEL TYPES
	res::createInventory();

	// GLFW
	initPhysimosWindow();
	physimosReturnWindowPointer();
	if (physimosWindowOK() == 0) {
		return -1;
	}

	PInput::init();

	// SIMULATORS
	// Make sure we initialize the simulator BEFORE grabbing it when setting up Simulator continer in the world scene
	Sim::init();
	Sim::compute();

	// TEMPORARY SETUP CALL LOCATION
	shader_init_shaders();

	PScene::init();

	UI::init();

	

	return 0;
}

void conductor_main(){

	while (windowIsStillGood())
	{
		
		new_frame();
		// glClear(GL_COLOR_BUFFER_BIT);
		// glClear(GL_DEPTH_BUFFER_BIT);


		// FPS INFO
		timing_newFrame();


		// Check is escape is pressed
		processInput();


		PScene::updateCurrentScene();
		PScene::renderCurrentScene();


		// update and render ui
		UI::update();
		


		// Swap buffers
		endOfFrameGlfwCalls();


		// Force wait to limit FPS to specified constant
		timing_waitForNextFrame();

	}

	terminatePhysimosWindow();

}





