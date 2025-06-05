
#include "conductor2D.hh"



Conductor2D::Conductor2D(f2 window_size_f) 
	: 	auxwin {window_size_f}
{

	this->window_size_f = window_size_f;

	// TODO: get content scale parameter
	pui.set_window_info(window_size_f, {1,1});

	reload_camera_root();

	// Phont renderer
	opengl::build_program_vert_frag(opengl::ProgramName::ndc_black);

}





void Conductor2D::input_window_change(InputEvent& event){

	WindowResizeEvent& resize_event = event.window_resize;

	window_size_f = resize_event.size_f;


	reload_camera_root();

	pui.set_window_info(window_size_f, {1.0f,1.0f});

	// ROOT SCENE
	scene::Scene2D* root_scene = ManagerScene::get_scene(root_scene_id);
	if(root_scene == nullptr)
		return;

	root_scene->set_window_size(window_size_f);
	


}



void Conductor2D::input_scroll(InputEvent & event){

	MouseScrollEvent& scroll_event = event.mouse_scroll;

	if(targeting_ui){
	
	}
	else {
	
		if(current_scene_target != nullptr)
			current_scene_target->handle_scroll(scroll_event.delta);

	}


}

void Conductor2D::input_mouse_move(InputEvent & event){

	MouseMoveEvent& mouse_movement = event.mouse_movement;
	CursorPosition& cursor_new = event.cursor;
	CursorPosition& cursor_prev = event.mouse_movement.cursor_prev;

	cursor_pos = cursor_new;

	// Pass move event recursively
	if(targeting_ui){

		pui.move_event(mouse_movement.delta.sane);
		
	}
	else { // Scene

		
			// current_scene_target->handle_pointer_move(scroll_event.delta);

		scene::Scene2D* root_scene = ManagerScene::get_scene(root_scene_id);
		
		scene::PointerMovement2D scene_pointer_move;
		scene_pointer_move.pos_prev = cursor_prev.normalized;
		scene_pointer_move.pos_curr = cursor_new.normalized;

		root_scene->handle_pointer_move(scene_pointer_move);



	}


}

void Conductor2D::input_mouse_button(InputEvent & event){
	
	CursorPosition& cursor = event.cursor;
	window::MouseButtonEvent mouse_button_event = event.mouse_button;

	if(targeting_ui){
		
		if(mouse_button_event.is_left_down())
			pui.event_mouse_down_current_target();
		else if(mouse_button_event.is_left_up())
			pui.event_mouse_up_current_target();
		
		return;
	}
	

	scene::Scene2D* root_scene = ManagerScene::get_scene(root_scene_id);
	
	scene::PointerClick2D pointer_click = {
		cursor.normalized,
		mouse_button_event
	};

	root_scene->handle_pointer_click(pointer_click);

	
}



void Conductor2D::update_root_scene(){

	scene::Scene2D* root_scene = ManagerScene::get_scene(root_scene_id);
	if(root_scene == nullptr)
		throw std::runtime_error("Root scene is nullptr");

	root_scene->update();
	root_scene->render_subscene_textures();

	auxwin.bind_window_framebuffer();
	root_scene->render_to_window();

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
	scene::Scene2D* root_scene = ManagerScene::get_scene(root_scene_id);
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
		


		auxwin.bind_window_framebuffer();

		// Root Scene
		update_root_scene();


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


