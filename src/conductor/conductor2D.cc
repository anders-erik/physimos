
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
	
	// cursor_pos.print();
	

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



void Conductor2D::
render_quad_textures()
{
	auto& q_manager = ManagerScene::get_quad_manager();

	for(QuadS2D& quad : q_manager.get_quads_mut())
	{
		if(quad.is_bitmap())
		{
			// nothing yet
		}
		else if (quad.is_scene2D())
		{
			// Get scene
			size_t scene_id = quad.get_object_id();
			scene::Scene2D* scene_p = ManagerScene::search_scene_storage(scene_id);
			if(scene_p == nullptr) continue;
			auto& scene = *scene_p;

			if(scene.is_multisampled())
			{
				unsigned int text_id = renderer_scene.render_scene_FBMS(scene, scene.get_FBMS());
				quad.set_texture_id(text_id);
			}
			else
			{
				unsigned int text_id = renderer_scene.render_scene_FBMS(scene, scene.get_FBMS());
				quad.set_texture_id(text_id);
				
			}


		}
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
		render_quad_textures();

		

		// RENDER

		auxwin.bind_window_framebuffer();
		renderer_scene.render_scene(window_scene);


		pui.update(); // reflect scene state changes
		pui.render(); // Render ui on top of scenes
		
		auxwin.end_frame();
	}

	auxwin.destroy();
}





