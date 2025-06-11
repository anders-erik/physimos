
#pragma once


#include "window/auxwin.hh"

#include "ui/pui.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/shapeS2D.hh"

#include "scene/render/renderer2D.hh"


using namespace window;

/** 
	Owns an auxwin, pui, and a scene manager.
	Handles events and decides which root scene will recieve input events.
*/
class Conductor2D 
{
	window::Auxwin auxwin;

	UI::PUI pui;

	bool targeting_ui = false;
	CursorPosition cursor_pos; // Copy from most recent mouse move event

	scene::RendererScene2D renderer; // TODO: remove when no implicit OpenGL dependencies remain

public:

	Conductor2D(f2 window_size);

	/** Set flag and pass cursor location. */
	void target_pui();
	/** Set flag and pass cursor location to scene manager. */
	void target_scenes();
	/** Make sure that we know what subsystem we are currently targeting. (Scene or UI)*/
	void set_subsystem_target();

	/** Mouse and keyboard input. */
	void process_mouse_movement();
	/** Mouse and keyboard input. */
	void process_other_events();
	/** Window resize, content scale, etc. */
	void process_framebuffer_events();

	/** transfer control to Conductor2D by entering main rendering loop */
	void main_loop();
};


