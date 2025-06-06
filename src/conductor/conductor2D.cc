
#include "conductor2D.hh"



Conductor2D::Conductor2D(f2 window_size_f) 
	: 	auxwin {window_size_f}
{

	// TODO: get content scale parameter
	pui.set_window_info(window_size_f, {1,1});

	// reload_camera_root();

	// First scene added to scene manager becomes root
	scene::Scene2D* root_scene = ManagerScene::init(window_size_f);
	root_scene->set_camera_width(window_size_f.x);

	// Phont rendering program
	opengl::build_program_vert_frag(opengl::ProgramName::ndc_black);

}



void Conductor2D::update_current_target(){

	// UI
	pui.update_current_targets(cursor_pos.sane);
	// Scenes
	scene::Scene2D* root_scene = ManagerScene::get_root_scene();

	targeting_ui = pui.has_target() ? true : false;

	if(!targeting_ui)
	{
		// current_scene_target = root_scene->try_find_target_scene(cursor_pos.normalized);
		ManagerScene::update_current_target(cursor_pos.normalized);
	}
	else 
	{
		ManagerScene::clear_current_target();
		// current_scene_target = nullptr;
	}
}



void Conductor2D::input_scroll(InputEvent & event){

	MouseScrollEvent& scroll_event = event.mouse_scroll;

	if(targeting_ui){
		pui.event_scroll(scroll_event.delta);
	}
	else {

		scene::Scene2D* current_target = ManagerScene::get_current_target();
		if(current_target != nullptr)
			current_target->handle_scroll(scroll_event.delta);

		// if(current_scene_target != nullptr)
		// 	current_scene_target->handle_scroll(scroll_event.delta);

	}


}

void Conductor2D::input_mouse_move(InputEvent & event){

	MouseMoveEvent& mouse_movement = event.mouse_movement;
	CursorPosition& cursor_new = event.cursor;
	CursorPosition& cursor_prev = event.mouse_movement.cursor_prev;

	cursor_pos = cursor_new;

	// Pass move event recursively
	if(targeting_ui)
	{

		pui.event_move(mouse_movement.delta.sane);

		// current_scene_target = nullptr;
	}
	else // Scene
	{

		// current_scene_target->handle_pointer_move(scroll_event.delta);

		scene::Scene2D* current_target = ManagerScene::get_current_target();
		if(current_target == nullptr)
			return;
		
		
		scene::PointerMovement2D scene_pointer_move;
		scene_pointer_move.pos_prev = cursor_prev.normalized;
		scene_pointer_move.pos_curr = cursor_new.normalized;

		scene::Scene2D* root_scene = ManagerScene::get_root_scene();
		root_scene->handle_pointer_move(scene_pointer_move);

	}


}

void Conductor2D::input_mouse_button(InputEvent & event){
	
	CursorPosition& cursor = event.cursor;
	window::MouseButtonEvent mouse_button_event = event.mouse_button;

	if(targeting_ui){
		
		if(mouse_button_event.is_left_down())
			pui.event_mouse_down();
		else if(mouse_button_event.is_left_up())
			pui.event_mouse_up();
		
		return;
	}
	

	scene::Scene2D* root_scene = ManagerScene::get_root_scene();
	
	scene::PointerClick2D pointer_click = {
		cursor.normalized,
		mouse_button_event
	};

	root_scene->handle_pointer_click(pointer_click);

	
}


void Conductor2D::input_window_change(InputEvent& event){

	WindowResizeEvent& resize_event = event.window_resize;

	pui.set_window_info(resize_event.size_f, {1.0f,1.0f});

	ManagerScene::get_root_scene()->set_window_size(resize_event.size_f);

}






void Conductor2D::new_frame(){

	auxwin.end_frame();
	input_events = auxwin.new_frame();

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

		// Always start out with trying to figure out what we're currently targeting
		update_current_target();
		
		// Now we know what we're targeting. This is now where we send events
		process_user_input();


		
		scene::Scene2D* root_scene = ManagerScene::get_root_scene();

		
		root_scene->update();

		root_scene->render_subscene_textures();

		// Render To Window
		auxwin.bind_window_framebuffer();
		root_scene->render_to_window();

		// UI
		pui.render();

		process_framebuffer_events();
		
	}

	auxwin.destroy();
}





