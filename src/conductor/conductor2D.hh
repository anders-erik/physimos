
#pragma once


#include "window/auxwin.hh"

#include "ui/pui.hh"

#include "scene/manager.hh"
#include "scene/scene2D.hh"
#include "scene/shapeS2D.hh"

#include "scene/render/renderer2D.hh"
#include "scene/render3D/renderer3D.hh"

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

	scene::RendererScene2D renderer_scene_2D;
	RendererScene3D renderer_scene_3D;

public:

	Physimos(f2 window_size);

	/** Mouse and keyboard input. */
	void process_user_input();
	/** Window resize, content scale, etc. */
	void process_framebuffer_events();
	/** Render all quad textures */
	void render_quad_textures();

	void update();
	void render();

	/** transfer control to Physimos by entering main rendering loop */
	void main_loop();
};


