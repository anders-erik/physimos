
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
process_user_input()
{
	auto events = auxwin.get_events_input();

	for(InputEvent& event : events)
	{
		// Short circuit logic if grab flags are set
		if(pui.is_grabbing_cursor())
		{
			ManagerScene::clear_cursor_hovers();
			pui.event_all(event);
			continue;
		}
		else if (ManagerScene::is_grabbing_cursor())
		{
			pui.clear_hovers();
			ManagerScene::events_user_all(event);
			continue;
		}


		// Requeries 
		if(pui.contains_point(cursor_pos.sane))
		{
			ManagerScene::clear_cursor_hovers();
			pui.event_all(event);
		}
		else
		{
			pui.clear_hovers();
			ManagerScene::requery_cursor_target(cursor_pos);
			ManagerScene::events_user_all(event);
		}

		
		// Cache most up to date cursor position
		if(event.is_mouse_movement())
		{
			cursor_pos = event.mouse_movement.cursor_new;
		}
	}
}


void Conductor2D::
process_framebuffer_events()
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

		process_user_input();

		// UPDATE
		scene::Scene2D& window_scene = ManagerScene::get_window_scene_mut();
		window_scene.update();
		window_scene.render_subscene_textures();

		pui.update(); // reflect scene state changes
		

		// RENDER

		auxwin.bind_window_framebuffer();
		window_scene.render_to_window();
		pui.render(); // Render ui on top of scenes
		
		auxwin.end_frame();
	}

	auxwin.destroy();
}





