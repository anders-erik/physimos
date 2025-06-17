
#include "conductor2D.hh"



Conductor2D::Conductor2D(f2 window_size_f) 
	: 	auxwin {window_size_f},
		pui {UI::PUI(window_size_f, auxwin.get_monitor_content_scale())}
{
	// First scene added to scene manager becomes root
	Scene3D& root_scene = ManagerScene::init(window_size_f);
	// root_scene.set_viewbox_width(window_size_f.x);

}







void Conductor2D::
process_user_input()
{
	process_framebuffer_events(); // change framebuffer size, content scale

	auto events = auxwin.get_events_input();
	
	// cursor_pos.print();
	

	for(InputEvent& event : events)
	{

		if(event.is_mouse())
		{
			// mouse grab
			if(input_state.is_grabbing_mouse())
			{
				if(input_state.pui())
				{
					ManagerScene::clear_cursor_hovers();
					auto response = pui.event_all(event);
					input_state.update(GrabStateConductor::PUI, response);
					if(!input_state.is_grabbing_mouse())
						std::cout << "pui release" << std::endl;

					continue;
				}
				else if (ManagerScene::is_grabbing_cursor())
				{
					pui.clear_hovers();
					auto response = ManagerScene::events_user_all(event);
					if(!input_state.is_grabbing_mouse())
						std::cout << "scenes release" << std::endl;
					continue;
				}
			}


			// Requery if cursor is not grabbed

			if(pui.contains_point(cursor_pos.sane))
			{
				auto response = pui.event_all(event);
				input_state.update(GrabStateConductor::PUI, response);
				if(input_state.is_grabbing_mouse())
					std::cout << "pui grab" << std::endl;

				ManagerScene::clear_cursor_hovers();
			}
			else
			{
				// ManagerScene::requery_cursor_target(cursor_pos);
				auto response = ManagerScene::events_user_all(event);
				input_state.update(GrabStateConductor::SCENES, response);
				if(input_state.is_grabbing_mouse())
					std::cout << "scenes grab" << std::endl;

				pui.clear_hovers();
			}

			// Cache most up to date cursor position
			if(event.is_mouse_movement())
			{
				cursor_pos = event.mouse_movement.cursor_new;
			}
		}


		// Currently only the scene recieved keyboard input
		if(event.is_keystroke())
		{
			ManagerScene::events_user_all(event);
		}
	}
}


void Conductor2D::
process_framebuffer_events()
{
	auto resize_events = auxwin.get_events_window_resize();

	for(WindowResizeEvent& event : resize_events)
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
			// // Get scene
			// size_t scene_id = quad.get_object_id();
			// scene::Scene2D* scene_p = ManagerScene::search_scene_storage(scene_id);
			// if(scene_p == nullptr) continue;
			// auto& scene = *scene_p;

			// if(scene.is_multisampled())
			// {
			// 	unsigned int text_id = renderer_scene_2D.render_scene_FBMS(scene, scene.get_FBMS());
			// 	quad.set_texture_id(text_id);
			// }
			// else
			// {
			// 	unsigned int text_id = renderer_scene_2D.render_scene_FBMS(scene, scene.get_FBMS());
			// 	quad.set_texture_id(text_id);
				
			// }


		}
	}
}



void Conductor2D::update()
{
	pui.update(); // reflect scene state changes
}

void Conductor2D::render()
{
	SceneBase& window_scene = ManagerScene::get_window_scene_mut();
	render_quad_textures();
	auxwin.bind_window_framebuffer();

	if(window_scene.is_2d())
		renderer_scene_2D.render_scene((scene::Scene2D&)window_scene);
	if(window_scene.is_3d())
		renderer_scene_3D.render_scene_3d(window_scene);

	pui.render(); // Render ui on top of scenes
}


void Conductor2D::main_loop()
{
	while (auxwin.is_open())
	{
		auxwin.new_frame();

		process_user_input();

		render();
		
		auxwin.end_frame();
	}

	auxwin.destroy();
}





