
#pragma once

#include "scene/bc.hh"

#include "window/auxwin.hh"

#include "scene/scene2D.hh"
#include "scene/subscene2D.hh"
#include "scene/shapeS2D.hh"


using namespace window;

/** 
	Owns a window and root 2D scenes.
	Handles events and decides which root scene will recieve input events.
*/
class Win2D {
	window::Auxwin auxwin;
	f2 window_size_f;

	scene::Camera2D camera_root; // Always displays the root scene filling the window
	opengl::Scene2DRenderer renderer;
	

	scene::SubScene2D* subscene_current_hover = nullptr;
	std::vector<scene::SubScene2D> subscenes; // sub-scenes owned by Win2D
	std::vector<scene::Scene2D> scenes; // Root scenes owned by Win2D
	// std::vector<scene::ShapeS2D> quads;
	scene::QuadS2D quad0;

	// To be replaced with BC tag
	scene::Scene2D* cursor_owner = nullptr;

	std::vector<window::InputEvent> input_events;

public:
	scene::Scene2D root_scene;
	BC::Tag root_scene_tag;


	Win2D(f2 window_size);

	// transfer control to Win2D by entering main rendering loop
	void start_loop();

	void input_scroll(InputEvent& event);
	void input_mouse_move(InputEvent& event);
	void input_mouse_button(InputEvent& event);
	void process_input(InputEvent & event);
	
	void set_window_size(f2 size);

	scene::SubScene2D& add_subscene(f2 pos_scene, f2 size_scene);


private:
	void reload_camera_root();
	void render_root();

	f2 transform_normalized_to_window(f2 normalized);
	f2 transform_window_to_normalized(f2 window);
};

