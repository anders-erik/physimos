
#include "conductor2D.hh"



Conductor2D::Conductor2D(f2 window_size_f) 
	: 	auxwin {window_size_f},
		pui {UI::PUI(window_size_f, auxwin.get_monitor_content_scale())}
{
	// First scene added to scene manager becomes root
	scene::Scene2D* root_scene = ManagerScene::init(window_size_f);
	root_scene->set_viewbox_width(window_size_f.x);

	// Phont rendering program
	opengl::build_program_vert_frag(opengl::ProgramName::ndc_black);
}

void Conductor2D::target_pui(){
	targeting_ui = true;
	ManagerScene::clear_cursor_highlighting();
}
void Conductor2D::target_scenes(){
	targeting_ui = false;
	ManagerScene::update_current_target(cursor_pos);
}

void Conductor2D::update_current_target()
{

	// Grab takes precedence over hover
	if(pui.is_grabbing_cursor())
	{
		target_pui();
	}
	else if(ManagerScene::is_grabbing_cursor())
	{
		target_scenes();
	}
	else if(pui.is_targeted_by_cursor())
	{
		target_pui();
	}
	else // if no grab nor ui target -> must be targeting a scene
	{
		target_scenes();
	}

}



void Conductor2D::
input_scroll(InputEvent & event)
{
	if(targeting_ui)
		pui.event_mouse_scroll(event);
	else
		ManagerScene::event_scroll(event);
}


void Conductor2D::
input_mouse_move(InputEvent & event)
{
	cursor_pos = event.cursor;

	if(targeting_ui)
		pui.event_mouse_move(event);
	else 
		ManagerScene::event_move(event);
}


void Conductor2D::
input_mouse_button(InputEvent & event)
{
	if(targeting_ui)
		pui.event_mouse_button(event);
	else
		ManagerScene::event_mouse_button(event);
}


void Conductor2D::
input_keystroke(InputEvent & event)
{
	if(targeting_ui)
		return;
	else
		ManagerScene::event_keystroke(event);
}



void Conductor2D::
input_window_change(InputEvent& event)
{
	// All subsystems get the resize event

	pui.event_window_resize(event);

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
		{
			// input_mouse_move(event);
			cursor_pos = event.cursor;

			if(targeting_ui)
				pui.event_all(event);
			else 
				ManagerScene::event_move(event);
		}
	}

	
	// Now that we know what subsystem we are targeting, we can process other inputs
	for(InputEvent& event : input_events)
	{

		if(event.is_type(window::EventType::MouseScroll))
		{
			// input_scroll(event);
			if(targeting_ui)
				pui.event_all(event);
			else
				ManagerScene::event_scroll(event);
		}
		else if(event.is_type(window::EventType::MouseButton))
		{
			// input_mouse_button(event);
			if(targeting_ui)
				pui.event_all(event);
			else
				ManagerScene::event_mouse_button(event);
		}
		else if(event.is_type(window::EventType::Keystroke))
		{
			// input_keystroke(event);
			if(targeting_ui)
				pui.event_all(event);
			else
				ManagerScene::event_keystroke(event);
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

		pui.reload(); // reflect scene state changes form previous frame
		pui.reload_cursor_target(cursor_pos.sane);

		// Always start out with trying to figure out which subsystem we're currently targeting
		update_current_target();
		
		// Now we know what we're targeting, we send the new input events
		process_user_input();


		
		scene::Scene2D& root_scene = ManagerScene::get_root_scene_mut();

		root_scene.update();

		root_scene.render_subscene_textures();

		// Render To Window
		auxwin.bind_window_framebuffer();
		root_scene.render_to_window();

		// UI
		pui.render();

		process_framebuffer_events();
		
	}

	auxwin.destroy();
}





