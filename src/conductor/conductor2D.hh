
#pragma once


#include "window/auxwin.hh"

#include "ui/pui.hh"

#include "scene/manager.hh"
#include "scene2D/scene2D.hh"
#include "scene2D/shapeS2D.hh"

#include "rend/scene2D/renderer2D.hh"
#include "rend/scene3D/renderer3D.hh"

#include "conductor2D_grabstate.hh"


using namespace window;


/**
	Owns an auxwin, pui, and a scene manager.
	Handles events and decides which root scene will recieve input events.
*/
class Physimos 
{
	window::Auxwin auxwin;
	UI::PUI pui;

	bool targeting_ui = false;
	GrabStateConductor input_state;
	CursorPosition cursor_pos; // Copy from most recent mouse move event


public:

	Physimos(int width, int height);

	bool is_quad_capture_click(InputEvent& event);
	bool should_release_quad(InputEvent& event);

	void send_event_scene(InputEvent& event);
	void send_event_quad(InputEvent& event);

	/** Mouse and keyboard input. */
	void process_mouse_input(InputEvent& event);
	/** Mouse and keyboard input. */
	void process_keyboard_input(InputEvent& event);

	/** Mouse and keyboard input. */
	void process_user_input();
	/** Window resize, content scale, etc. */
	void process_framebuffer_events();


	void update();

	void render();

	/** transfer control to Physimos by entering main rendering loop */
	void main_loop();

};


