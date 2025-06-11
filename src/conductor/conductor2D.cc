
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






void Conductor2D::
target_pui()
{
	targeting_ui = true;
	
}


void Conductor2D::
target_scenes()
{
	targeting_ui = false;
	pui.clear_cursor_target();
}


void Conductor2D::
set_subsystem_target()
{
	// Grab takes precedence over hover
	if(pui.is_grabbing_cursor())
	{
		target_pui();
		return;
	}
	else if(ManagerScene::is_grabbing_cursor())
	{
		target_scenes();
		return;
	}

	// Cold queries as no system is grabbing
	ManagerScene::clear_cursor_hovers();
	pui.clear_cursor_target();

	// Check is there is a pui match
	pui.set_cursor_pos_bypass_grab(cursor_pos.sane);
	if(pui.is_targeted_by_cursor())
	{
		targeting_ui = true;
		return;
	}

	// By process of elimination, scene is targeted

	targeting_ui = false;
	ManagerScene::set_cursor_pos_bypass_grab(cursor_pos);
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
		process_framebuffer_events(); // change framebuffer size before clearing
		auxwin.new_frame();


		// Set the target that will recieve all events during this frame
		// This query will use the cached mouse position from most recent movement event
		// Start out with this call to get stable access to objects that might have move at end of frame cleanup
		set_subsystem_target();

		process_mouse_movement();
		process_other_events();

		// UPDATE
		scene::Scene2D& root_scene = ManagerScene::get_root_scene_mut();
		root_scene.update();
		root_scene.render_subscene_textures();

		pui.update(); // reflect scene state changes
		

		// RENDER

		auxwin.bind_window_framebuffer();
		root_scene.render_to_window();
		pui.render(); // Render ui on top of scenes
		
		auxwin.end_frame();
	}

	auxwin.destroy();
}





