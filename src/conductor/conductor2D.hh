
#pragma once


#include "window/auxwin.hh"

#include "ui/pui.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/shapeS2D.hh"

#include "scene/render/renderer2D.hh"

#include "conductor2D_grabstate.hh"


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
	GrabStateConductor grab_state;
	CursorPosition cursor_pos; // Copy from most recent mouse move event

	scene::RendererScene2D renderer_scene;

public:

	Conductor2D(f2 window_size);

	/** Mouse and keyboard input. */
	void process_user_input();
	/** Window resize, content scale, etc. */
	void process_framebuffer_events();
	/** Render all quad textures */
	void render_quad_textures();

	/** transfer control to Conductor2D by entering main rendering loop */
	void main_loop();
};


