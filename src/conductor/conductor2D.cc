
#include "conductor2D.hh"



Conductor2D::Conductor2D(f2 window_size_f) 
	: 	auxwin {window_size_f}
{

	this->window_size_f = window_size_f;

	// TODO: get content scale parameter
	pui.set_window_info(window_size_f, {1,1});

	reload_camera_root();

	opengl::build_program_vert_frag(opengl::ProgramName::ndc_black);


}

f2 Conductor2D::transform_normalized_to_window(f2 normalized){
	return { normalized.x * window_size_f.x, normalized.y * window_size_f.y};
}

f2 Conductor2D::transform_window_to_normalized(f2 window){
	return { window.x / window_size_f.x, window.y / window_size_f.y};
}



scene::SubScene2D& Conductor2D::add_subscene(f2 pos_normalized, f2 size_normalized){

	f2 pos_window = transform_normalized_to_window(pos_normalized);
	f2 size_window = transform_normalized_to_window(size_normalized);
	
	scene::SubScene2D& new_subscene = subscenes.emplace_back(size_window);
	new_subscene.quad.set_box(pos_window, size_window);

	return new_subscene;
}


void Conductor2D::input_window_change(InputEvent& event){

	WindowResizeEvent& resize_event = event.window_resize;

	window_size_f = resize_event.size_f;



	reload_camera_root();

	pui.set_window_info(window_size_f, {1.0f,1.0f});

	// ROOT SCENE
	OptPtr<scene::Scene2D> root_scene_opt = BC::borrow_scene(root_scene_tag);
	if(root_scene_opt.has_value()){
		scene::Scene2D& _root_scene = root_scene_opt.get_ref();
		_root_scene.set_window_size(window_size_f);
		BC::return_scene(root_scene_tag);
	}

	// scene.set_window_size(event.window_resize.size_f);

	// for(scene::Scene2D& scene : scenes)
	// 	scene.set_window_size(size);
}



void Conductor2D::input_scroll(InputEvent & event){
	MouseScrollEvent& scroll_event = event.mouse_scroll;
	// Only one scene at the moment
	// scene::Scene2D& scene = scenes[0];

	if(targeting_ui){
	
	}
	else {
	
		if(current_scene_target != nullptr)
			current_scene_target->handle_scroll(scroll_event.delta);

		// SUBSCENE - LEGACY
		// scene::Scene2D* root_scene = BC::get_scene(root_scene_tag.id);
		// root_scene->handle_scroll(scroll_event.delta);

		if(subscenes[0].quad.contains_cursor(event.cursor.sane)){
			// std::cout << "SCROLL IN SCENE" << std::endl;
			// subscenes[0].scene.handle_scroll(scroll_event.delta);
			scene::Scene2D* subscene_0_scene = BC::get_scene(subscenes[0].scene_tag.id);
			subscene_0_scene->handle_scroll(scroll_event.delta);
		}
		// camera.zoom(event.mouse_scroll.delta);
	}


}

void Conductor2D::input_mouse_move(InputEvent & event){
	MouseMoveEvent& mouse_movement = event.mouse_movement;
	CursorPosition& cursor_new = event.cursor;
	CursorPosition& cursor_prev = event.mouse_movement.cursor_prev;

	// cursor_pos = cursor;

	// cursor_new.print();
	// mouse_movement.delta.sane.print("Delta [sane]: ");

	// targeting_ui = pui.find_new_target(cursor_pos.sane);

	// Pass move event recursively
	if(targeting_ui){

		pui.move_event(mouse_movement.delta.sane);
		
	}
	else { // Scene

		
			// current_scene_target->handle_pointer_move(scroll_event.delta);

		scene::Scene2D* root_scene = BC::get_scene(root_scene_tag.id);
		
		scene::PointerMovement2D scene_pointer_move;
		scene_pointer_move.pos_prev = cursor_prev.normalized;
		scene_pointer_move.pos_curr = cursor_new.normalized;

		root_scene->handle_pointer_move(scene_pointer_move);

		// LAGACY FOR RAW SUBSCENES

		if(subscenes[0].quad.contains_cursor(cursor_new.sane)){

			scene::Scene2D* subscene_0_scene = BC::get_scene(subscenes[0].scene_tag.id);

			subscene_current_hover = &subscenes[0];

			// Convert to normalized quad coordinates (= normal. subscene coords.)
			scene::PointerMovement2D pointer_movement;
			pointer_movement.pos_prev = subscenes[0].quad.get_normalized_from_point(cursor_prev.sane);
			pointer_movement.pos_curr = subscenes[0].quad.get_normalized_from_point(cursor_new.sane);

			// subscenes[0].scene.handle_pointer_move( pointer_movement );
			subscene_0_scene->handle_pointer_move( pointer_movement );
		}
		else {
			subscene_current_hover = nullptr;
		}


	}

	cursor_pos = cursor_new;

}

void Conductor2D::input_mouse_button(InputEvent & event){
	
	CursorPosition& cursor = event.cursor;
	window::MouseButtonEvent mouse_button_event = event.mouse_button;
	// mouse_button_event.button

	if(targeting_ui){
		
		if(mouse_button_event.is_left_down())
			pui.event_mouse_down_current_target();
		else if(mouse_button_event.is_left_up())
			pui.event_mouse_up_current_target();

	}
	else{

		scene::Scene2D* root_scene = BC::get_scene(root_scene_tag.id);
		
		scene::PointerClick2D pointer_click = {
			cursor.normalized,
			mouse_button_event
		};

		root_scene->handle_pointer_click(pointer_click);


		// LEGACY FOR RAW SUBSCENES
	
		if(subscenes[0].quad.contains_cursor(cursor.sane)){

			scene::Scene2D* subscene_0_scene = BC::get_scene(subscenes[0].scene_tag.id);

			scene::PointerClick2D pointer_click {
				pointer_click.pos_scene_normal = subscenes[0].quad.get_normalized_from_point(cursor.sane),
				pointer_click.button_event = mouse_button_event 
			};
			
			// subscenes[0].scene.handle_pointer_click( pointer_click );
			subscene_0_scene->handle_pointer_click(pointer_click);
		}

	}
}



void Conductor2D::update_subscenes(){

	// Render subscenes to textures and bind to quad
	scene::Scene2D* subscene_0_scene = BC::get_scene(subscenes[0].scene_tag.id);
	subscene_0_scene->update();
	subscenes[0].quad.set_texture_id(subscene_0_scene->render_to_texture());
	// subscenes[0].scene.update();
	// subscenes[0].quad.set_texture_id(subscenes[0].scene.render_to_texture());



	auxwin.bind_window_framebuffer();

	renderer.activate();
	renderer.render_quad(subscenes[0].quad);


	if(subscene_current_hover != nullptr)
		renderer.render_frame(subscene_current_hover->quad.get_matrix());

}

void Conductor2D::update_root_scene(){

	// old_root_scene.update();
	// old_root_scene.render();


	auto root_scene_opt = BC::get_scene(root_scene_tag);
	if(root_scene_opt.is_null())
		return;

	scene::Scene2D& root_scene_tmp = root_scene_opt.get_ref();

	root_scene_tmp.update();
	root_scene_tmp.render_subscene_textures();

	auxwin.bind_window_framebuffer();
	root_scene_tmp.render_to_window();

}

void Conductor2D::new_frame(){

	auxwin.end_frame();
	input_events = auxwin.new_frame();

}

void Conductor2D::update_current_target(){

	// Always start out with trying to figure out what were currently targeting

	// if we are currently grabbing an element, nothing to do
	if(pui.has_grabbed_target()){
		targeting_ui = true; 
		return;
	}

	pui.try_find_new_target(cursor_pos.sane);
	targeting_ui = pui.has_target();
	if(targeting_ui)
		return;


	// Query scenes
	scene::Scene2D* root_scene = BC::get_scene(root_scene_tag.id);
	current_scene_target = root_scene->try_find_current_scene(cursor_pos.normalized);


}

void Conductor2D::process_user_input(){


	// Handle mouse movement first
	for(InputEvent& event : input_events){
		if(event.is_type(window::EventType::MouseMove)){
			input_mouse_move(event);
			// break;
		}
	}

	
	// Now that we know what subsystem we are targeting, we can process other inputs
	for(InputEvent& event : input_events){

		if(event.is_type(window::EventType::MouseScroll)){
			input_scroll(event);
		}
		else if(event.is_type(window::EventType::MouseButton)){
			input_mouse_button(event);
		}

	}


}

void Conductor2D::process_framebuffer_events(){

	// Resize later in loop
	for(InputEvent& event : input_events){
		if(event.is_type(window::EventType::WindowResize)){
			input_window_change(event);
			break;
		}
	}

}


void Conductor2D::main_loop(){

	
	while (auxwin.is_open()){

		new_frame();


		update_current_target();
		

		process_user_input();
		

		// for(InputEvent& event : input_events)
		// 	process_input(event);
		
		update_subscenes();


		auxwin.bind_window_framebuffer();

		// Root Scene
		update_root_scene();
		

		// scene.set_window_size(window_size_f);
		// scene.render_window();

		

		auxwin.bind_window_framebuffer();
		pui.render();


		process_framebuffer_events();
		
	}

	auxwin.destroy();
}




void Conductor2D::reload_camera_root(){
	renderer.activate();
	camera_root.set_window_size_px(window_size_f);
	camera_root.set_width(window_size_f.x);
	renderer.set_camera(camera_root.get_matrix());
}


