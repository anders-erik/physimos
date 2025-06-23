
#include "rend/rend_manager.hh"

#include "conductor2D.hh"



Physimos::
Physimos(int width, int height) 
{
	auxwin.init(width, height);

	Rend::Manager::init( {width, height} );

	manager_3D.init();
}



Scene3D & Physimos::
get_window_scene()
{
	return *manager_3D.window_scene;
}





void Physimos::
send_event_pui(InputEvent & event)
{
	auto response = pui.event_all(manager_3D, event);
	mouse_grab.update(MouseGrab::PUI, response);
	
	manager_3D.state.active_tags.hover_clear();
}


void Physimos::
send_event_quad(InputEvent& event)
{
	auto* capturing_quado = manager_3D.manager_o.get_squado(manager_3D.state.active_tags.get_quad_capture());
	if(capturing_quado == nullptr)
	{
		manager_3D.state.active_tags.release_quad();
		mouse_grab.release();
		return;
	}

	// SEND EVENT
	if(capturing_quado->squad.is_scene2D())
	{
		Scene2D* scene_p = manager_2D.search_scene_storage_2D(capturing_quado->squad.sid);
		
		if(event.is_mouse_scroll())
			scene_p->handle_scroll(event);
		if(event.is_mouse_button())
			scene_p->handle_pointer_click(event);
		if(event.is_mouse_movement())
			scene_p->handle_pointer_move(event);

		mouse_grab.update(MouseGrab::QUAD, {InputResponse::MOUSE_GRAB});
		return;
	}

}


void Physimos::
send_event_scene(InputEvent & event)
{
	auto response = manager_3D.state.handle_mouse(	manager_3D, 
													event		);
	mouse_grab.update(MouseGrab::SCENE, response);
}



bool Physimos::
try_send_event_to_grabbed(InputEvent & event)
{
	switch (mouse_grab.state)
	{

	case MouseGrab::PUI:
		send_event_pui(event);
		break;
	
	case MouseGrab::QUAD:
		Rend::Manager::get_renderer_scene3D().sample_and_set_hover(manager_3D, event.cursor_pos.sane);
		if(manager_3D.state.try_release_quad(event))
		{
			mouse_grab.release();
			return false;
		}
		else
		{
			send_event_quad(event);
		}
		break;

	case MouseGrab::SCENE:
		send_event_scene(event);
		break;

	case MouseGrab::EMPTY:
		return false;
		break;
	}

	return true;
}



bool Physimos::
requery_pui(CursorPosition cursor_pos)
{
	pui.clear_hovers();
	if(pui.contains_point(cursor_pos.sane))
		return true;

	return false;
}


bool Physimos::
query_for_quad_grab(InputEvent& event, CursorPosition cursor_pos)
{
	Rend::Manager::get_renderer_scene3D().sample_and_set_hover(manager_3D, cursor_pos.sane);
	if(manager_3D.state.try_new_quad_grab(event))
		return true;

	return false;
}






void Physimos::
process_user_input()
{
	auto events = auxwin.get_events_input();

	for(InputEvent& event : events)
	{
		if(event.is_mouse())
		{
			if(try_send_event_to_grabbed(event))
				continue;

			if(requery_pui(event.cursor_pos))
				send_event_pui(event);
			else if(query_for_quad_grab(event, event.cursor_pos))
				mouse_grab.update(MouseGrab::QUAD, InputResponse::MOUSE_GRAB);
			else
				send_event_scene(event);

			if(event.is_mouse_movement())
				cursor_pos = event.mouse_movement.cursor_new;

			continue;
		}

		if(event.is_keystroke()) // Currently only the scene recieves keyboard input
		{
			manager_3D.state.handle_key(manager_3D, event);
		}
	}
}


void Physimos::
process_framebuffer_events()
{
	auto resize_events = auxwin.get_events_window_resize();

	for(WindowResizeEvent& window_event : resize_events)
	{
		// All subsystems get the resize event
		pui.event_window_resize(window_event);

		manager_3D.state.handle_window(	manager_3D,
										window_event);

		Rend::Manager::get_renderer_scene3D().set_window_fb_size(window_event);
	}
}



void Physimos::
update()
{
	pui.update(	manager_3D ); // reflect scene state changes
}



void Physimos::
render()
{
	Rend::Manager::get_renderer_scene2D().render_all_scene2D_to_frambuffers(manager_2D);

	auxwin.bind_window_framebuffer();

	auto& rend_scene3D = Rend::Manager::get_renderer_scene3D();

	rend_scene3D.render_scene_3d(  *manager_3D.window_scene, 
									manager_3D				);

	rend_scene3D.render_object_ids(*manager_3D.window_scene,
									manager_3D				);
	
	pui.render(); // Render ui on top of scene
}



void Physimos::
main_loop()
{
	while (auxwin.is_open())
	{
		auxwin.new_frame();

		process_framebuffer_events(); // change framebuffer size, content scale
		process_user_input();

		update();

		render();
		
		auxwin.end_frame();
	}

	auxwin.destroy();
}





