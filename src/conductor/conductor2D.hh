
#pragma once


#include "window/auxwin.hh"

#include "ui/pui.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/subscene2D.hh"
#include "scene/shapeS2D.hh"


using namespace window;

/** 
	Owns an auxwin, pui, and a scene manager.
	Handles events and decides which root scene will recieve input events.
*/
class Conductor2D {

	window::Auxwin auxwin;

	// True conductor states

	CursorPosition cursor_pos;
	bool targeting_ui = false;

	opengl::Scene2DRenderer renderer;

	// Move to InputManager - split into separate event arrays / separate auxwin calls
	std::vector<window::InputEvent> input_events;


	UI::PUI pui;


public:

	Conductor2D(f2 window_size);

	void target_pui();
	void target_scenes();
	/** Makes sure that we know what subsystem we are currently targeting. (Scene or UI)*/
	void update_current_target();

	// Input
	void input_scroll(InputEvent& event);
	void input_mouse_move(InputEvent& event);
	void input_mouse_button(InputEvent& event);
	void input_keystroke(InputEvent& event);
	void input_window_change(InputEvent& event);
	/** Mouse and keyboard input. */
	void process_user_input();
	/** Window resize, content scale, etc. */
	void process_framebuffer_events();
	
	/** Swap buffers, set up new frame, and load input event into conductor. */
	void new_frame();

	/** transfer control to Conductor2D by entering main rendering loop */
	void main_loop();


};


