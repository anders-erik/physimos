
#include "win2D.hh"



Win2D::Win2D(f2 window_size_f) 
	: auxwin {window_size_f} 
{

	this->window_size_f = window_size_f;

	reload_camera_root();

	opengl::build_program_vert_frag(opengl::Programs::ndc_black);


}


scene::Scene2D& Win2D::add_subscene(f2 pos_normalized, f2 size_normalized){

	scene::Scene2D& scene = scenes.emplace_back(window_size_f);
	scene.set_window_norm_box(pos_normalized, size_normalized);

	// quad0.set_dims({10.0f, 10.0f}, {100.0f, 100.0f});
	quad0.set_box({50.0f, 200.0f}, {window_size_f.x/2.0f, window_size_f.y/2.0f});
	renderer.create_context_quad_t(quad0.get_rendering_context(), quad0.get_verts());

	return scene;
}


void Win2D::set_window_size(f2 size){
	for(scene::Scene2D& scene : scenes)
		scene.set_window_size(size);
}


void Win2D::start_loop(){

	while (auxwin.end_frame()){
		start_frame();

		scene::Scene2D& scene = scenes[0];
		
		// scene.print();

		// UPDATE
		scene.update();

		// RENDER
		quad0.set_texture_id(scene.render_to_texture());

		auxwin.bind_framebuffer();
		// scene.set_window_size(window_size_f);
		// scene.render_window();

		
		
		render_root();
	}

	auxwin.destroy();
}



void Win2D::input_scroll(InputEvent & event){
	MouseScrollEvent& scroll_event = event.mouse_scroll;
	// Only one scene at the moment
	scene::Scene2D& scene = scenes[0];
	if(quad0.contains_cursor(event.cursor.sane)){
		// std::cout << "SCROLL IN SCENE" << std::endl;
		scene.handle_scroll(scroll_event.delta);
	}
	// camera.zoom(event.mouse_scroll.delta);

}

void Win2D::input_mouse_move(InputEvent & event){
	// MouseMoveEvent& mouse_movement = event.mouse_movement;
	CursorPosition& cursor_prev = event.mouse_movement.cursor_prev;
	CursorPosition& cursor = event.cursor;

	scene::Scene2D& scene = scenes[0];

	if(quad0.contains_cursor(cursor.sane)){

		// Convert to normalized quad coordinates (= normal. subscene coords.)
		scene::PointerMovement2D pointer_movement;
		pointer_movement.pos_prev = quad0.get_normalized_from_point(cursor_prev.sane);
		pointer_movement.pos_curr = quad0.get_normalized_from_point(cursor.sane);

		scene.handle_pointer_move( pointer_movement );
	}
}


void Win2D::process_input(InputEvent& event){
		
	// Only one scene at the moment
	scene::Scene2D& scene = scenes[0];

	switch (event.type){

	case EventType::WindowResize:
		window_size_f = event.window_resize.size_f;
		reload_camera_root();
		// scene.set_window_size(event.window_resize.size_f);
		break;

	case EventType::MouseMove:
		input_mouse_move(event);
		break;

	case EventType::MouseScroll:
		input_scroll(event);
		break;
	
	default:
		// std::cout << "WARN: unhandled input event" << std::endl;
		scene.handle_input(event);
		break;
	}

}


void Win2D::start_frame(){

	input_events = auxwin.new_frame();

	for(InputEvent& event : input_events)
		process_input(event);

}

void Win2D::reload_camera_root(){
	renderer.activate();
	camera_root.set_window_size_px(window_size_f);
	camera_root.set_width(window_size_f.x);
	renderer.set_camera(camera_root.get_matrix());
}
void Win2D::render_root(){
	auxwin.bind_framebuffer();
	renderer.activate();
	renderer.set_model(quad0.get_matrix());
	renderer.render_quad(quad0.get_rendering_context());
}

