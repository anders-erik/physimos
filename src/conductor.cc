
#include "Windowing.hpp"
#include "shader.hpp"
#include "scene/pscene.hh"
#include "ui/ui.hh"
#include "Timing.hpp"

#include "Input.hpp"

#include "Simulator.hpp"

#include "resources.hh"

#include "lib/lib.hh"
#include "lib/log.hh"

#include "conductor.hh"

#include "conductor_internal.hh"

#include "ui/ui_primitive.hh"
#include "ui/ui_globals.hh"


/** The only valid grid object holding the current UI grid state and is also used for updating the ui. */
UI::Grid current_grid;


/** Returned primitive from finding primitive target during left click. Is reset to nullptr on left release. */
UI::Primitive* grabbed_primitive = nullptr;

/** Current cursor position recieved in the input callback function. */
double cursor_x = 0.0;
/** Current cursor position recieved in the input callback function. y = 0 at the bottom of window.  */
double cursor_y = 0.0;

UI::Primitive* currentlyHoveredPrimitive = nullptr;



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

	// Subscribe to cursor position from input library
    ::PInput::subscribe_pointer_position_conductor(callback_pointer_position);
    ::PInput::subscribe_mouse_left_down_conductor(callback_left_down);
    ::PInput::subscribe_mouse_left_release_conductor(callback_left_release);
    ::PInput::subscribe_mouse_scroll_y_conductor(callback_scroll_y);



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



void conductor_perform_action(CAction action){

	switch (action)
	{
	case CAction::UI_ToggleLeftPanel :
		current_grid.left_panel_visible = current_grid.left_panel_visible ? false : true;
		UI::set_ui_grid(current_grid);
		break;
	
	case CAction::UI_ToggleWorkbench :
		current_grid.workbench_visible = current_grid.workbench_visible ? false : true;
		UI::set_ui_grid(current_grid);
		break;
	
	case CAction::UI_ToggleRightPanel :
		current_grid.right_panel_visible = current_grid.right_panel_visible ? false : true;
		UI::set_ui_grid(current_grid);
		break;
	
	default:
		break;
	}
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








void callback_pointer_position(PInput::PointerPosition pointer_pos, PInput::PointerChange _pointer_change) {
	// std::cout << "CBC POSITION" << std::endl;
	
    // UI STATE
    cursor_x = pointer_pos.x;
    cursor_y = pointer_pos.y;

    // DRAG
    if(grabbed_primitive != nullptr){
        grabbed_primitive->grabbed(_pointer_change.dx, _pointer_change.dy);
    }
        
    // RESET HOVER
    if (currentlyHoveredPrimitive != nullptr) {
        currentlyHoveredPrimitive->hover_exit();
        currentlyHoveredPrimitive = nullptr;
    }

    // TRY NEW HOVER
    UI::UiResult targetResult = UI::try_find_target(cursor_x, cursor_y);
    if (targetResult.success) {
        currentlyHoveredPrimitive = targetResult.primitive;
        targetResult.primitive->hover_enter();
		return;
    }

	// TRY NEW HOVER
    targetResult = UI::try_find_target_old(cursor_x, cursor_y);
    if (targetResult.success) {
        currentlyHoveredPrimitive = targetResult.primitive;
        targetResult.primitive->hover_enter();
		return;
    }

}


void callback_scroll_y(double y_change) {
    
    UI::UiResult scrollTargetQuery;

    // SCROLL FOUND TARGET DIRECTLY
    scrollTargetQuery = UI::try_find_target(cursor_x, cursor_y);
    if (scrollTargetQuery.success) {

        if(scrollTargetQuery.primitive->scrollable){
            scrollTargetQuery.primitive->scroll(y_change);

		}
    }
    // BUBBLE TARGET
    if (scrollTargetQuery.success) {

        // Simply scroll if target is scrollable
        if(scrollTargetQuery.primitive->scrollable){
            scrollTargetQuery.primitive->scroll(y_change);
            return;
        }

        // If not scrollable, bubble ui tree until root primitive is found.
        // If at any point a scrollable primtiive is encountered, the scroll method is called
        UI::Primitive* currentPrimitive = scrollTargetQuery.primitive;
        while(currentPrimitive->parent != nullptr){
            currentPrimitive = currentPrimitive->parent;

            if(currentPrimitive->scrollable){
                currentPrimitive->scroll(y_change);
                // break;
				return;
            }
        }       
        
    }



	// EDITORS
    scrollTargetQuery = UI::try_find_target_old(cursor_x, cursor_y);
    if (scrollTargetQuery.success) {

        if(scrollTargetQuery.primitive->scrollable)
            scrollTargetQuery.primitive->scroll(y_change);
    }
    // BUBBLE TARGET (EDITORS)
    if (scrollTargetQuery.success) {

        // Simply scroll if target is scrollable
        if(scrollTargetQuery.primitive->scrollable){
            scrollTargetQuery.primitive->scroll(y_change);
            return;
        }

        // If not scrollable, bubble ui tree until root primitive is found.
        // If at any point a scrollable primtiive is encountered, the scroll method is called
        UI::Primitive* currentPrimitive = scrollTargetQuery.primitive;
        while(currentPrimitive->parent != nullptr){
            currentPrimitive = currentPrimitive->parent;

            if(currentPrimitive->scrollable){
                currentPrimitive->scroll(y_change);
                break;
            }
        }       
        
    }
    
}

void callback_left_release(PInput::PointerPosition _pointer_pos){

    // We are only interested in release behavior if we targeted a primitive on left click
    if (grabbed_primitive == nullptr)
        return;
    
    UI::UiResult releaseTargetResult;
    bool click_confirmed;


    // TRIGGER CLICK
    releaseTargetResult = UI::try_find_target(cursor_x, cursor_y);
	
    // We click if: 1) released on primitive, 2) primitive is the same one as registered on left down
    click_confirmed = releaseTargetResult.success && releaseTargetResult.primitive == grabbed_primitive;
    if (click_confirmed) {
        plog_info(::plib::LogScope::UI, "Click registered on " + releaseTargetResult.primitive->id);
        UI::UiResult click_result = releaseTargetResult.primitive->click();

		if(click_result.action != CAction::None)
			conductor_perform_action(click_result.action);

        // NEVER PERSIST GRAB ON RELEASE
        grabbed_primitive = nullptr;
        return;
    }


	// PRIMITIVE EDITOR STUFF
	releaseTargetResult = UI::try_find_target_old(cursor_x, cursor_y);
	
    // We click if: 1) released on primitive, 2) primitive is the same one as registered on left down
    click_confirmed = releaseTargetResult.success && releaseTargetResult.primitive == grabbed_primitive;
    if (click_confirmed) {
        plog_info(::plib::LogScope::UI, "Click registered on " + releaseTargetResult.primitive->id);
        UI::UiResult click_result = releaseTargetResult.primitive->click();

		if(click_result.action != CAction::None)
			conductor_perform_action(click_result.action);

        // NEVER PERSIST GRAB ON RELEASE
        grabbed_primitive = nullptr;
        return;
    }

    
    
    // NEVER PERSIST GRAB ON RELEASE
    grabbed_primitive = nullptr;
    return;
    
}

void callback_left_down(PInput::PointerPosition _pointer_pos) {

    UI::UiResult clickTargetResult;


    // REGISTER FOR CLICK ON RELEASE
    clickTargetResult = UI::try_find_target(cursor_x, cursor_y);
    if(clickTargetResult.success){
        grabbed_primitive = clickTargetResult.primitive;
        // plog_info(::plib::LogScope::UI, "Grabbed : " + grabbed_primitive->id);
        return;
    }

	clickTargetResult = UI::try_find_target_old(cursor_x, cursor_y);
    if(clickTargetResult.success){
        grabbed_primitive = clickTargetResult.primitive;
        // plog_info(::plib::LogScope::UI, "Grabbed : " + grabbed_primitive->id);
        return;
    }

}