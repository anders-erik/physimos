
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

	// To be replaced with BC tag
	scene::Scene2D* cursor_owner = nullptr;

	UI::PUI pui;
	CursorPosition cursor_pos;

public:
	BC::Tag root_scene_tag;


	Conductor2D(f2 window_size);


	void input_window_change(InputEvent& event);
	void input_scroll(InputEvent& event);
	void input_mouse_move(InputEvent& event);
	void input_mouse_button(InputEvent& event);
	void process_input(InputEvent & event);
	
	void update_subscenes();
	void update_root_scene();

	// transfer control to Conductor2D by entering main rendering loop
	void main_loop();

	scene::SubScene2D& add_subscene(f2 pos_scene, f2 size_scene);


private:
	void reload_camera_root();
	void render_root();

	f2 transform_normalized_to_window(f2 normalized);
	f2 transform_window_to_normalized(f2 window);
};

