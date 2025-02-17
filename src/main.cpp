#include <iostream>

#include "process_info.hpp"
#include "Windowing.hpp"
#include "shader.hpp"
#include "scene/pscene.hh"
// #include "ui_old.hpp"
#include "ui/ui.hh"
#include "Timing.hpp"


#include "Simulator.hpp"

// io tests
#include "resources.hh"

// logger
#include "logger.hh"



#include "lib/lib.hh"
extern std::string physimos_root_dir;

int main()
{
	physimos_root_dir_or_die();

	// Plog
	// plog(MAIN, "Hello from main.cpp");
	// plog(MAIN, ENTERED_MAIN, "Another physimos start.");
	// plog_error(IMAGE, LogError::OBJ_PARSING, "Dummy error log message.");

	// std::cout << std::endl << "---------- MAIN() ----------" << std::endl << std::endl;
	
	// 2024-09-13
	std::cout << std::endl << "Consistent incremental improvement is the fundamental action of anything worthwhile. " << std::endl;
	std::cout              << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ " << std::endl << std::endl;


	
	srand(0);


	// TEMP FOR PNG DEV!
	// png::loadFile("./media/4black.png");
	// png::prettyPrint();
	// return 0;

	// LOGGER
	logger::init();
	

	// START LOADING RESOURCES - ATM ONLY GETS MODEL TYPES
	res::createInventory();


	// glfw window creation
	// --------------------
	initPhysimosWindow();
	physimosReturnWindowPointer();
	if (physimosWindowOK() == 0) {
		return -1;
	}



	// START SIMULATORS
	// Make sure we initialize the simulator BEFORE grabbing it when setting up Simulator continer in the world scene
	Sim::init();
	Sim::compute();

	// TEMPORARY SETUP CALL LOCATION
	shader_init_shaders();

	// ws_init();
	PScene::init();


	UI::init();
	// ui_init();


	process_info_init();



	/* 
	
		MAIN LOOP
	
	*/

	while (windowIsStillGood())
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



		// ws_update();
		// ws_render();
		PScene::updateCurrentScene();
		PScene::renderCurrentScene();


		//
		// update and render ui
		//
		UI::update();
		// ui_update();
		


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


	terminatePhysimosWindow();

	return 0;
}





