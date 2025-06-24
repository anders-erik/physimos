
#include "rend/rend_manager.hh"

#include "conductor2D.hh"



Physimos::
Physimos(int width, int height) 
{
	auxwin.init(width, height);

	Rend::Manager::init( {width, height} );

	manager_3D.init( {width, height} );
}



Scene3D & Physimos::
get_window_scene()
{
	return *manager_3D.window_scene;
}

void Physimos::
update_grab(MouseGrab::State subsystem, InputResponse response)
{
	mouse_grab.update(subsystem, response);

	if(mouse_grab.pui())
		auxwin.set_cursor_state(Cursor::HAND);
	else if(mouse_grab.quad())
		auxwin.set_cursor_state(Cursor::INVERTED);
	else if(mouse_grab.scene())
		auxwin.set_cursor_state(Cursor::CAPTURE);
	else
		auxwin.set_cursor_state(Cursor::NORMAL);	
}





void Physimos::
send_event_pui(InputEvent & event)
{
	auto response = pui.event_all(manager_3D, event);
	update_grab(MouseGrab::PUI, response);
	
	manager_3D.state.active_tags.hover_clear();
}


void Physimos::
send_event_quad(InputEvent& event)
{
	auto response = manager_3D.manager_q.handle_event(	event,
														manager_2D	);
	update_grab(MouseGrab::QUAD, response);
}


void Physimos::
send_event_scene(InputEvent & event)
{
	// A scene grab always has a virtual cursor position at the center of the window
	// manager_3D.renderer_3D.sample_and_set_hover(manager_3D, auxwin.get_window_size_float() / 2 );

	auto response = manager_3D.state.handle_user_input(	manager_3D, 
														event,		
														sampled_tag	);
	update_grab(MouseGrab::SCENE, response);
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
		if(manager_3D.manager_q.try_release_quad(event, sampled_tag))
		{
			update_grab(MouseGrab::QUAD, InputResponse::MOUSE_RELEASE);
			return false;
		}
		send_event_quad(event);
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

			if(pui.contains_point(event.cursor_pos.sane))
				send_event_pui(event);
			else if(manager_3D.manager_q.try_new_quad_grab(event, sampled_tag))
				update_grab(MouseGrab::QUAD, InputResponse::MOUSE_GRAB);
			else
				send_event_scene(event);

			continue;
		}

		if(event.is_keystroke())
		{
			// Always scene unless explicitly grabbed by others
			if(!try_send_event_to_grabbed(event))
				send_event_scene(event);
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

		manager_3D.renderer_3D.set_window_fb_size(window_event);
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

	manager_3D.render_window_scene(	auxwin.get_window_size(),
									auxwin.get_cursor_pos()	);

	sampled_tag = manager_3D.renderer_3D.sample_oid_tag(  	manager_3D.window_scene->tagos,
                                                			auxwin.get_cursor_pos().sane     	);

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





