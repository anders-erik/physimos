
#pragma once

#include "window/auxwin.hh"

#include "scene/scene2D.hh"
#include "scene/shapeS2D.hh"


using namespace window;

/** 
	Owns a window and root 2D scenes.
	Handles events and decides which root scene will recieve input events.
*/
class Win2D {
	window::Auxwin auxwin;

	scene::Camera2D camera_root; // Always displays the root scene filling the window
	opengl::Scene2DRenderer renderer;
	// std::vector<scene::ShapeS2D> quads;
	scene::QuadS2D quad0;

	// To be replaced with BC tag
	scene::Scene2D* cursor_owner = nullptr;

	f2 window_size_f;
	std::vector<scene::Scene2D> scenes; // Root scenes owned by Win2D
	std::vector<window::InputEvent> input_events;

public:

	Win2D(f2 window_size);

	scene::Scene2D& add_subscene(f2 pos_normalized, f2 size_normalized);
	void set_window_size(f2 size);

	// transfer control to Win2D by entering main rendering loop
	void start_loop();
    // Set up new frame and handle input
	void start_frame();

	void input_scroll(InputEvent& event);
	void input_mouse_move(InputEvent& event);
	void process_input(InputEvent & event);

private:
	void reload_camera_root();
	void render_root();
};

