
#include "win2D.hh"



Win2D::Win2D(f2 window_size_f) 
	: auxwin {window_size_f}
{

	this->window_size_f = window_size_f;

	reload_camera_root();

	opengl::build_program_vert_frag(opengl::ProgramName::ndc_black);


}

f2 Win2D::transform_normalized_to_window(f2 normalized){
	return { normalized.x * window_size_f.x, normalized.y * window_size_f.y};
}

f2 Win2D::transform_window_to_normalized(f2 window){
	return { window.x / window_size_f.x, window.y / window_size_f.y};
}



scene::SubScene2D& Win2D::add_subscene(f2 pos_normalized, f2 size_normalized){

	f2 pos_window = transform_normalized_to_window(pos_normalized);
	f2 size_window = transform_normalized_to_window(size_normalized);

	scene::SubScene2D& new_subscene = subscenes.emplace_back(size_window);
	new_subscene.set_box(pos_window, size_window);

	return new_subscene;
}


void Win2D::set_window_size(f2 size){
	for(scene::Scene2D& scene : scenes)
		scene.set_window_size(size);
}


void Win2D::start_loop(){

	while (auxwin.end_frame()){

		input_events = auxwin.new_frame();

		for(InputEvent& event : input_events)
			process_input(event);


		// scene::Scene2D& scene = scenes[0];
		scene::SubScene2D& subscene = subscenes[0];
		
		// scene.print();

		// UPDATE
		// scene.update();
		subscenes[0].update();

		// RENDER
		// quad0.set_texture_id(scene.render_to_texture());
		subscenes[0].set_texture_id(subscenes[0].render());

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
	if(subscenes[0].contains_cursor(event.cursor.sane)){
		// std::cout << "SCROLL IN SCENE" << std::endl;
		subscenes[0].handle_scroll(scroll_event.delta);
	}
	// camera.zoom(event.mouse_scroll.delta);

}

void Win2D::input_mouse_move(InputEvent & event){
	// MouseMoveEvent& mouse_movement = event.mouse_movement;
	CursorPosition& cursor_prev = event.mouse_movement.cursor_prev;
	CursorPosition& cursor = event.cursor;

	scene::Scene2D& scene = scenes[0];

	if(subscenes[0].contains_cursor(cursor.sane)){

		// Convert to normalized quad coordinates (= normal. subscene coords.)
		scene::PointerMovement2D pointer_movement;
		pointer_movement.pos_prev = subscenes[0].get_normalized_from_point(cursor_prev.sane);
		pointer_movement.pos_curr = subscenes[0].get_normalized_from_point(cursor.sane);

		subscenes[0].handle_pointer_move( pointer_movement );
	}
}

void Win2D::input_mouse_button(InputEvent & event){
	
	CursorPosition& cursor = event.cursor;
	window::MouseButtonEvent mouse_button_event = event.mouse_button;
	// mouse_button_event.button
	
	if(subscenes[0].contains_cursor(cursor.sane)){

		scene::PointerClick2D pointer_click {
			pointer_click.pos_scene_normal = subscenes[0].get_normalized_from_point(cursor.sane),
			pointer_click.button_event = mouse_button_event 
		};
		
		subscenes[0].handle_pointer_click( pointer_click );
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

	case EventType::MouseButton:
		input_mouse_button(event);
		break;

	default:
		// std::cout << "WARN: unhandled input event" << std::endl;
		scene.handle_input(event);
		break;
	}

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
	renderer.render_quad(subscenes[0].get_quad());

}

