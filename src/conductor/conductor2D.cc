
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

void Conductor2D::target_pui(){
	targeting_ui = true;
	ManagerScene::clear_current_target();
}
void Conductor2D::target_scenes(){
	targeting_ui = false;
	ManagerScene::update_current_target(cursor_pos);
}

void Conductor2D::update_current_target()
{
	pui.reload_current_targets(cursor_pos.sane);

	// Grab takes precedence over hover
	if(pui.has_grabbed_target())
	{
		target_pui();
	}
	else if(ManagerScene::has_grabbed_target())
	{
		target_scenes();
	}
	else if(pui.has_hover_target())
	{
		target_pui();
	}
	else // if no grab nor ui target -> must be targeting a scene
	{
		target_scenes();
	}

}



void Conductor2D::input_scroll(InputEvent & event){

	MouseScrollEvent& scroll_event = event.mouse_scroll;

	if(targeting_ui)
		pui.event_scroll(scroll_event.delta);
	else
		ManagerScene::event_scroll(event);

}

void Conductor2D::input_mouse_move(InputEvent & event)
{
	cursor_pos = event.cursor;

	MouseMoveEvent& mouse_movement = event.mouse_movement;

	if(targeting_ui)
		pui.event_move(mouse_movement.delta.sane);
	else 
		ManagerScene::event_move(event);

}


void Conductor2D::
input_mouse_button(InputEvent & event)
{
	window::MouseButtonEvent mouse_button_event = event.mouse_button;

	if(targeting_ui){
		
		if(mouse_button_event.is_left_down())
			pui.event_mouse_down();
		else if(mouse_button_event.is_left_up())
			pui.event_mouse_up();
		
		return;
	}
	else
		ManagerScene::event_mouse_button(event);

}

void Conductor2D::input_keystroke(InputEvent & event)
{
	if(targeting_ui)
		return;
	else
		ManagerScene::event_keystroke(event);
}



void Conductor2D::input_window_change(InputEvent& event)
{

	WindowResizeEvent& resize_event = event.window_resize;

	pui.set_window_info(resize_event.size_f, {1.0f,1.0f});

	ManagerScene::event_window_resize(event);

}






void Conductor2D::new_frame(){

	auxwin.end_frame();
	input_events = auxwin.new_frame();

}


void Conductor2D::process_user_input(){


	// Handle mouse movement first
	for(InputEvent& event : input_events)
	{
		if(event.is_type(window::EventType::MouseMove))
			input_mouse_move(event);
	}

	
	// Now that we know what subsystem we are targeting, we can process other inputs
	for(InputEvent& event : input_events)
	{

		if(event.is_type(window::EventType::MouseScroll))
		{
			input_scroll(event);
		}
		else if(event.is_type(window::EventType::MouseButton))
		{
			input_mouse_button(event);
		}
		else if(event.is_type(window::EventType::Keystroke))
		{
			input_keystroke(event);
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

		pui.update(); // reflect scene state changes form previous frame

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





