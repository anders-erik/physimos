
#include "Windowing.hpp"
#include "window_cursors.hh"

#include "shader.hpp"
#include "scene/pscene.hh"
#include "ui/ui.hh"
#include "Timing.hpp"

#include "draw/draw.hh"

#include "Input.hpp"

#include "Simulator.hpp"

#include "io/resources.hh"

#include "lib/lib.hh"
#include "lib/log.hh"

#include "conductor.hh"
#include "conductor_common.hh"
#include "conductor_internal.hh"
#include "conductor_callbacks.hh"
#include "conductor_viewport.hh"

#include "ui/ui_primitive.hh"
#include "ui/ui_globals.hh"


/** The only valid grid object holding the current UI grid state and is also used for updating the ui. */
// UI::Grid current_grid;

StateMain state_main_default = StateMain::Draw;
StateMain state_main_current;


/** Returned primitive from finding primitive target during left click. Is reset to nullptr on left release. */
UI::Primitive* grabbed_primitive = nullptr;
UI::Primitive* hovered_primitive = nullptr;

bool main_state_cursor_grab = false;

/** Singleton viewport context */
static ViewportContext viewport_context;

/** Current cursor position recieved in the input callback function. */
// double cursor_x = 0.0;
/** Current cursor position recieved in the input callback function. y = 0 at the bottom of window.  */
// double cursor_y = 0.0;




int conductor_rouse()
{


	// START LOADING RESOURCES - ATM ONLY GETS MODEL TYPES
	res::createInventory();

	// GLFW
	initPhysimosWindow(VIEWPORT_WIDTH_INIT, VIEWPORT_HEIGHT_INIT);
	physimosReturnWindowPointer();
	if (physimosWindowOK() == 0) {
		return -1;
	}

	PhysWin new_window = get_initial_physimos_window();
	viewport_context.phys_win = new_window;

	state_main_current = state_main_default;

	viewport_context.set_left_panel_w(left_panel_default_width);
	viewport_context.set_right_panel_w(right_panel_default_width);
	viewport_context.set_workbench_h(workbench_default_height);
	viewport_context.update_heights();
	viewport_context.update_widths();


	PInput::init();
	subscribeWindowChange_conductor(callback_window_change); // WINDOWING.CPP

	// Subscribe to cursor position from input library
	::PInput::subscribe_pointer_position_conductor(callback_pointer_position);

	::PInput::subscribe_mouse_left_down_conductor(callback_left_down);
	::PInput::subscribe_mouse_left_release_conductor(callback_left_release);

	::PInput::subscribe_mouse_middle_down_conductor(callback_middle_down);
	::PInput::subscribe_mouse_middle_release_conductor(callback_middle_release);
	::PInput::subscribe_mouse_scroll_y_conductor(callback_scroll_y);

	::PInput::subscribe_mouse_backward_conductor(callback_mouse_backward);
	::PInput::subscribe_mouse_forward_conductor(callback_mouse_forward);

	::PInput::subscribe_key_down_conductor(callback_key_down);
	::PInput::subscribe_key_up_conductor(callback_key_up);



	// SIMULATORS
	// Make sure we initialize the simulator BEFORE grabbing it when setting up Simulator continer in the world scene
	Sim::init();
	Sim::compute();

	// TEMPORARY SETUP CALL LOCATION
	shader_init_shaders();

	PScene::init();

	UI::init();
	UI::state_main_set(state_main_current);
	// UI::set_ui_grid(current_grid);
	UI::set_ui_views(viewport_context.view_sizes, viewport_context.visibility);


	draw::init(viewport_context);

	return 0;
}

void conductor_conduct() {

	while (windowIsStillGood())
	{

		new_frame();
		// glClear(GL_COLOR_BUFFER_BIT);
		// glClear(GL_DEPTH_BUFFER_BIT);


		// FPS INFO
		timing_newFrame();


		// Check is escape is pressed
		processInput();

		if (viewport_context.viewport_changed)
			reload_viewport();


		PScene::updateCurrentScene();
		if (state_main_current == StateMain::Scene3D)
			PScene::renderCurrentScene();
		else if (state_main_current == StateMain::Draw)
			draw::draw();

		// update and render ui
		UI::update();




		// Swap buffers
		endOfFrameGlfwCalls();


		// Force wait to limit FPS to specified constant
		timing_waitForNextFrame();

	}

	terminatePhysimosWindow();

}

void state_main_set(StateMain new_state_main) {
	state_main_current = new_state_main;

	UI::state_main_set(new_state_main);
}

StateMain get_left_state() {
	if (state_main_current == StateMain::Scene3D)
		return StateMain::UIEditor;
	else if (state_main_current == StateMain::Draw)
		return StateMain::Scene3D;
	else if (state_main_current == StateMain::UIEditor)
		return StateMain::Draw;

	return state_main_default;
}

StateMain get_right_state() {
	if (state_main_current == StateMain::Scene3D)
		return StateMain::Draw;
	else if (state_main_current == StateMain::Draw)
		return StateMain::UIEditor;
	else if (state_main_current == StateMain::UIEditor)
		return StateMain::Scene3D;

	return state_main_default;
}


void conductor_perform_action(CAction action) {


	switch (action) {

		case CAction::UI_ToggleLeftPanel:
			viewport_context.toggle_left_panel();
			UI::set_ui_views(viewport_context.view_sizes, viewport_context.visibility);
			break;

		case CAction::UI_ToggleWorkbench:
			viewport_context.toggle_workbench();
			UI::set_ui_views(viewport_context.view_sizes, viewport_context.visibility);
			break;

		case CAction::UI_ToggleRightPanel:
			viewport_context.toggle_right_panel();
			UI::set_ui_views(viewport_context.view_sizes, viewport_context.visibility);
			break;

		case CAction::State_ToggleScene3D:
			state_main_set(StateMain::Scene3D);
			break;

		case CAction::State_ToggleCanvas:
			state_main_set(StateMain::Draw);
			break;

		case CAction::State_ToggleUIEditor:
			state_main_set(StateMain::UIEditor);
			break;

		case CAction::State_SwitchRight:
			state_main_set(get_right_state());
			break;

		case CAction::State_SwitchLeft:
			state_main_set(get_left_state());
			break;

	default:
		break;
	}


	// if UI action
	if(action > CAction::UI_MIN && action < CAction::UI_MAX){
		
		draw::update_window(viewport_context);
		UI::update_window(viewport_context.phys_win);

	}

}


void reload_viewport() {

	viewport_context.update_heights();
	viewport_context.update_widths();

	UI::set_ui_views(viewport_context.view_sizes, viewport_context.visibility);


	viewport_context.viewport_changed = false;
}

void callback_window_change(PhysWin new_window) {

	viewport_context.phys_win = new_window;

	reload_viewport();

	UI::update_window(new_window);
	draw::update_window(viewport_context);

}








void callback_pointer_position(ViewportCursor pointer_pos, ViewportCursor _pointer_change) {

	viewport_context.set_cursor(pointer_pos.x, pointer_pos.y);


	// Ui Dragging takes precedence over main view interactivity
	if (grabbed_primitive != nullptr) {

		if (grabbed_primitive->id == "UIC_Root_Workbench_Resizer") {
			viewport_context.accumulate_workbench(_pointer_change.y);
			// No other initiated ui events during grab!
			return;
		}
		else if (grabbed_primitive->id == "UIC_Root_LeftPanel_Resizer") {
			viewport_context.accumulate_left_panel(_pointer_change.x);
			// No other initiated ui events during grab!
			return;
		}
		else if (grabbed_primitive->id == "UIC_Root_RightPanel_Resizer") {
			viewport_context.accumulate_right_panel(_pointer_change.x);
			// No other initiated ui events during grab!
			return;
		}

		grabbed_primitive->grabbed(_pointer_change.x, _pointer_change.y);
	}

	// If cursor not grabbed, then we can look for ui elements to interact with
	if(!main_state_cursor_grab){


		// RESET UI HOVER
		if (hovered_primitive != nullptr) {
			UI::HoverEvent hover_event = hovered_primitive->hover_exit();

			set_cursor(hover_event.cursor);
			hovered_primitive = nullptr;

		}

		// TRY NEW UI HOVER
		UI::UiResult targetResult = UI::try_find_target(viewport_context.cursor_real.x, viewport_context.cursor_real.y);
		if (targetResult.success) {
			// hovered_primitive = targetResult.primitive;
			UI::HoverEvent hover_event = targetResult.primitive->hover_enter();

			if (hover_event.new_hover) {
				set_cursor(hover_event.cursor);
				hovered_primitive = hover_event.primitive;
			}

			return;
		}

		
	}


	
	// 
	switch (state_main_current){
		case StateMain::Draw :
			// if(middle_mouse_main_view_grab)
			draw::set_cursor_main_view(viewport_context.cursor_main_view);
			draw::cursor_move(_pointer_change);
			return;

			break;
		
		default:
			// plog_error("CONDUCTOR ", "MIDDLE_MOUSE_GRAB_STATE ", "A registered grab-state is present, but no matching state-case.");
			break;
		}

}


void callback_scroll_y(double y_change) {

	UI::UiResult scrollTargetQuery;

	// SCROLL FOUND TARGET DIRECTLY
	scrollTargetQuery = UI::try_find_target(viewport_context.cursor_real.x, viewport_context.cursor_real.y);
	if (scrollTargetQuery.success) {

		if (scrollTargetQuery.primitive->scrollable) {
			scrollTargetQuery.primitive->scroll(y_change);

		}
	}
	// BUBBLE TARGET
	if (scrollTargetQuery.success) {

		// Simply scroll if target is scrollable
		if (scrollTargetQuery.primitive->scrollable) {
			scrollTargetQuery.primitive->scroll(y_change);
			return;
		}

		// If not scrollable, bubble ui tree until root primitive is found.
		// If at any point a scrollable primtiive is encountered, the scroll method is called
		UI::Primitive* currentPrimitive = scrollTargetQuery.primitive;
		while (currentPrimitive->parent != nullptr) {
			currentPrimitive = currentPrimitive->parent;

			if (currentPrimitive->scrollable) {
				currentPrimitive->scroll(y_change);
				// break;
				return;
			}
		}

	}


	std::cout << "SCROLL MAIN"  << std::endl;
	switch (state_main_current)
	{
	case StateMain::Draw :
		draw::scroll(y_change);
		break;
	
	default:
		break;
	}
	

}

void callback_left_release(ViewportCursor _pointer_pos) {

	// Any grabbing should be released
	main_state_cursor_grab = false;
	switch (state_main_current){

	case StateMain::Draw :
		draw::left_btn_up();
		break;
	
	default:
		break;
	}

	// We are only interested in release behavior if we targeted a primitive on left click
	if (grabbed_primitive == nullptr)
		return;

	// RELEASE
	set_cursor(PCursor::Default);


	UI::UiResult releaseTargetResult;
	bool click_confirmed;


	// TRIGGER CLICK
	releaseTargetResult = UI::try_find_target(viewport_context.cursor_real.x, viewport_context.cursor_real.y);

	// We click if: 1) released on primitive, 2) primitive is the same one as registered on left down
	click_confirmed = releaseTargetResult.success && releaseTargetResult.primitive == grabbed_primitive;
	if (click_confirmed) {
		plog_info(::plib::LogScope::UI, "Click registered on " + releaseTargetResult.primitive->id);
		UI::UiResult click_result = releaseTargetResult.primitive->click();

		if (click_result.action != CAction::None)
			conductor_perform_action(click_result.action);

		// NEVER PERSIST GRAB ON RELEASE
		grabbed_primitive = nullptr;
		return;
	}


	// NEVER PERSIST GRAB ON RELEASE
	grabbed_primitive = nullptr;
	return;

}

void callback_left_down(ViewportCursor _pointer_pos) {
	// std::chrono::steady_clock::now();
	viewport_context.set_cursor(_pointer_pos.x, _pointer_pos.y);

	UI::UiResult clickTargetResult;


	// REGISTER FOR CLICK ON RELEASE
	clickTargetResult = UI::try_find_target(viewport_context.cursor_real.x, viewport_context.cursor_real.y);
	if (clickTargetResult.success) {

		UI::GrabState try_grab = clickTargetResult.primitive->grab();
		if (try_grab.grabbed) {
			// SET CURSOR
			set_cursor(try_grab.cursor);
			// SET GRABBED PRIMITIVE
			grabbed_primitive = try_grab.primitive;

			// If we grabbed a primitive, no other changes should be made
			return;
		}

		// grabbed_primitive = clickTargetResult.primitive;
		clickTargetResult.primitive->click();
		plog_info(::plib::LogScope::UI, "Click registered on " + clickTargetResult.primitive->id);

		UI::UiResult click_result = clickTargetResult.primitive->click();
		if (click_result.action != CAction::None)
			conductor_perform_action(click_result.action);

		// plog_info(::plib::LogScope::UI, "Grabbed : " + grabbed_primitive->id);
		return;
	}

	std::cout << "MAIN VIEW"  << std::endl;
	switch (state_main_current){

	case StateMain::Draw :
		draw::set_cursor_main_view(viewport_context.cursor_main_view);
		main_state_cursor_grab = draw::left_btn_down();
		break;
	
	default:
		break;
	}

}


void callback_middle_down(ViewportCursor _pointer_pos){

	switch (state_main_current){
	case StateMain::Draw :
		draw::set_cursor_main_view(viewport_context.cursor_main_view);
		main_state_cursor_grab = draw::middle_btn_down();
		break;
	
	default:
		break;
	}

	std::cout << "MIDDLE DOWN"  << std::endl;
}
void callback_middle_release(ViewportCursor _pointer_pos){
	main_state_cursor_grab = false;

	switch (state_main_current){
		case StateMain::Draw :
			draw::set_cursor_main_view(viewport_context.cursor_main_view);
			draw::middle_btn_up();
			break;
		
		default:
			break;
		}

	std::cout << "MIDDLE RELEASE"  << std::endl;
}


void callback_mouse_backward(ViewportCursor _pointer_pos){
	std::cout << "backward"  << std::endl;
	switch (state_main_current) {
		case StateMain::Draw :
			draw::mouse_backward();
			break;
		
		default:
			break;
	}
	
}
void callback_mouse_forward(ViewportCursor _pointer_pos){
	std::cout << "forward"  << std::endl;

	switch (state_main_current) {
		case StateMain::Draw :
			draw::mouse_forward();
			break;
		
		default:
			break;
	}
}

void callback_key_down(PInput::KeyEvent key_event) {
	// std::cout << "DOWN"  << std::endl;

	using namespace PInput;

	if (key_event.pkey == PKey::PageUp && key_event.modifier == KeyMod::CTRL ) {
		conductor_perform_action(CAction::State_SwitchLeft);
	}
	else if (key_event.pkey == PKey::PageDown && key_event.modifier == KeyMod::CTRL) {
		conductor_perform_action(CAction::State_SwitchRight);
	}
	else if (key_event.pkey == PKey::B && key_event.modifier == KeyMod::CTRL_ALT) {
		conductor_perform_action(CAction::UI_ToggleWorkbench);
	}
	else if (key_event.pkey == PKey::B && key_event.modifier == KeyMod::CTRL) {
		conductor_perform_action(CAction::UI_ToggleLeftPanel);
	}
	else if (key_event.pkey == PKey::B && key_event.modifier == KeyMod::ALT) {
		conductor_perform_action(CAction::UI_ToggleRightPanel);
	}
	else if (key_event.pkey == PKey::Z && key_event.modifier == KeyMod::CTRL) {
		// conductor_perform_action(CAction::UI_ToggleLeftPanel);
		std::cout << "undo"  << std::endl;
		
	}
	else if (key_event.pkey == PKey::Z && key_event.modifier == KeyMod::CTRL_SHIFT) {
		// conductor_perform_action(CAction::UI_ToggleRightPanel);
		std::cout << "redo"  << std::endl;
	}


}
void callback_key_up(PInput::KeyEvent key_event) {
	// std::cout << "UP"  << std::endl;

}