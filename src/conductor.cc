
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



/** The only valid grid object holding the current UI grid state and is also used for updating the ui. */
UI::Grid current_grid;




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
	subscribeWindowChange_conductor(callback_window_change); // WINDOWING.CPP

	// SIMULATORS
	// Make sure we initialize the simulator BEFORE grabbing it when setting up Simulator continer in the world scene
	Sim::init();
	Sim::compute();

	// TEMPORARY SETUP CALL LOCATION
	shader_init_shaders();

	PScene::init();

	UI::init();
	UI::set_ui_grid(current_grid);
	

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



void callback_window_change(PhysWin physimos_window) {

    // viewport_width = physimos_window.width / physimos_window.xscale;
    // viewport_height = physimos_window.height / physimos_window.yscale;

	UI::update_window(physimos_window);
    
    // // SHADER TRANSFORM
    // shader::texture_shader.set_window_info(
    //     physimos_window.width,
    //     physimos_window.height,
    //     physimos_window.xscale,
    //     physimos_window.yscale
    // );

    // shader::color_shader.set_window_info(
    //     physimos_window.width, 
    //     physimos_window.height, 
    //     physimos_window.xscale, 
    //     physimos_window.yscale
    // );

    // RELOAD ALL PRIMITIVES TO GET CORRECT DIMENSIONS
    // primitive_editor->update_w_real_recursive();
    // primitive_editor->update_x_real_recursive();

    // primitive_editor->update_h_real_recursive();
    // primitive_editor->update_y_real_recursive();
}



