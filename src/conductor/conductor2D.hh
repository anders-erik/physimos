
#pragma once


#include "window/auxwin.hh"

#include "ui/pui.hh"

#include "scene/manager_3D.hh"
#include "scene/scene_state.hh"
#include "rend/scene3D/renderer3D.hh"

#include "scene2D/manager_2D.hh"
#include "scene2D/scene2D.hh"
#include "scene2D/shapeS2D.hh"
#include "rend/scene2D/renderer2D.hh"

#include "conductor2D_grabstate.hh"


using namespace window;


/**
	Owns an auxwin, pui, and a scene manager.
	Handles events and decides which root scene will recieve input events.
*/
struct Physimos 
{

	window::Auxwin auxwin;
	UI::PUI pui;
	Manager3D manager_3D;
	Manager2D manager_2D;

	RendererScene2D 	rendererScene2D;
	UI::RendererBase    renderer_pui;

	PhysimosGrab grab;

	TagO sampled_tag = TagO(); // sampled tag at current cursor position immediately after window render

	Physimos(int width, int height);

	Scene3D& get_window_scene();

	void update_grab(PhysimosGrab::SubSystemBits subsystem, InputResponse response);

	void send_event_pui(InputEvent& event);
	void send_event_quad(InputEvent& event);
	void send_event_scene(InputEvent& event);

	/** Returns true on consumed event. */
	bool try_send_event_to_grabbed(InputEvent & event);

	/** Mouse and keyboard input. */
	void process_user_input();
	/** Window resize, content scale, etc. */
	void process_framebuffer_events();

	void update();

	void render();

	/** transfer control to Physimos by entering main rendering loop */
	void main_loop();

};



