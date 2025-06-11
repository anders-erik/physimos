
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

void Conductor2D::set_subsystem_target()
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






void Conductor2D::process_mouse_movement()
{
	auto movement_events = auxwin.get_events_mouse_movement();

	for(InputEvent& event : movement_events)
	{
		cursor_pos = event.mouse_movement.cursor_new;

		if(targeting_ui)
			pui.event_all(event);
		else 
			ManagerScene::event_move(event);
	}
}

void Conductor2D::process_other_events()
{
	auto events = auxwin.get_events_other();

	if(targeting_ui)
	{
		for(InputEvent& event : events)
		{
			pui.event_all(event);
		}
		return;
	}

	// As we are targeting a scene, we send the events to their appropriate handler
	for(InputEvent& event : events)
	{
		if(event.is_mouse_scroll())
		{
			ManagerScene::event_scroll(event);
		}
		else if(event.is_mouse_button())
		{
			ManagerScene::event_mouse_button(event);
		}
		else if(event.is_keystroke())
		{
			ManagerScene::event_keystroke(event);
		}
	}
}

void Conductor2D::process_framebuffer_events()
{
	auto resize_events = auxwin.get_events_window_resize();

	for(InputEvent& event : resize_events)
	{
		// All subsystems get the resize event
		pui.event_window_resize(event);
		ManagerScene::event_window_resize(event);
	}
}


void Conductor2D::main_loop()
{
	
	while (auxwin.is_open())
	{
		auxwin.new_frame();

		// Set the target that will recieve all events during this frame
		// This query will use the cached mouse position from most recent movement event
		// Start out with this call to get stable access to objects that might have move at end of frame cleanup
		set_subsystem_target();

		process_mouse_movement();
		process_framebuffer_events();
		process_other_events();

		// Root scene (to become window scene)
		scene::Scene2D& root_scene = ManagerScene::get_root_scene_mut();
		root_scene.update();
		root_scene.render_subscene_textures();
		auxwin.bind_window_framebuffer();
		root_scene.render_to_window();

		pui.reload(); // reflect scene state changes
		pui.reload_cursor_target(cursor_pos.sane);
		// Render ui after all scenes are rendered
		pui.render();

		auxwin.end_frame();
	}

	auxwin.destroy();
}





