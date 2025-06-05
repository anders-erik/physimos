
#pragma once


#include "window/auxwin.hh"

#include "ui/pui.hh"

#include "scene/bc.hh"
#include "scene/scene2D.hh"
#include "scene/subscene2D.hh"
#include "scene/shapeS2D.hh"


using namespace window;

/** 
	Owns a window and root 2D scenes.
	Handles events and decides which root scene will recieve input events.
*/
class Conductor2D {

	window::Auxwin auxwin;
	std::vector<window::InputEvent> input_events;
	f2 window_size_f;

	scene::Camera2D camera_root; // Always displays the root scene filling the window
	opengl::Scene2DRenderer renderer;
	

	scene::SubScene2D* subscene_current_hover = nullptr;
	std::vector<scene::SubScene2D> subscenes; // sub-scenes owned by Conductor2D


	UI::PUI pui;
	CursorPosition cursor_pos;
	bool targeting_ui = false;

public:
	BC::Tag root_scene_tag;
	scene::Scene2D* current_scene_target = nullptr;


	Conductor2D(f2 window_size);


	void input_window_change(InputEvent& event);
	void input_scroll(InputEvent& event);
	void input_mouse_move(InputEvent& event);
	void input_mouse_button(InputEvent& event);
	
	void update_subscenes();
	void update_root_scene();

	/** Swap buffers, set up new frame, and load input event into conductor. */
	void new_frame();
	/** Makes sure that we know what subsystem we are currently targeting before handling events */
	void update_current_target();

	/** Mouse and keyboard input. */
	void process_user_input();
	/** Window resize, content scale, etc. */
	void process_framebuffer_events();

	/** transfer control to Conductor2D by entering main rendering loop */
	void main_loop();

	scene::SubScene2D& add_subscene(f2 pos_scene, f2 size_scene);


private:

	void reload_camera_root();

	f2 transform_normalized_to_window(f2 normalized);
	f2 transform_window_to_normalized(f2 window);
};

