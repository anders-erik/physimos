

#include "conductor2D.hh"



Physimos::
Physimos(int width, int height) 
{
	auxwin.init(width, height);

	// Rend::Manager::init( {width, height} );
	renderer_pui.init();
	renderer_pui.set_window_info(	auxwin.get_window_fb_size_float(), 
									auxwin.get_monitor_content_scale()	);
	rendererScene2D.init();

	manager_3D.init( {width, height} );
}



Scene3D & Physimos::
get_window_scene()
{
	return *manager_3D.window_scene;
}

void Physimos::
update_grab(PhysimosGrab::SubSystemBits subsystem, InputResponse response)
{
	grab.update(subsystem, response);

	switch (grab.state)
	{

	case PhysimosGrab::QUAD_MOUSE_GRAB :
		auxwin.set_cursor_state(Cursor::INVERTED);
		break;

	case PhysimosGrab::SCENE_MOUSE_GRAB :
	case PhysimosGrab::PUI_MOUSE_GRAB :
		auxwin.set_cursor_state(Cursor::CAPTURE);
		break;

	case PhysimosGrab::SCENE_MOUSE_PAN :
		auxwin.set_cursor_state(Cursor::HAND);
		break;

	default:
		auxwin.set_cursor_state(Cursor::NORMAL);
		break;
	
	}
}





void Physimos::
send_event_pui(InputEvent & event)
{
	auto response = pui.event_all(	manager_3D, 
									event, 
									auxwin.get_cursor_pos().pixels	);

	update_grab(PhysimosGrab::PUI, response);
	
	manager_3D.state.hovered.clear();
}


void Physimos::
send_event_quad(InputEvent& event)
{
	auto response = manager_3D.manager_q.handle_event(	event,
														manager_2D	);
	update_grab(PhysimosGrab::QUAD, response);
}


void Physimos::
send_event_scene(InputEvent & event)
{
	// A scene grab always has a virtual cursor position at the center of the window
	// manager_3D.renderer_3D.sample_and_set_hover(manager_3D, auxwin.get_window_size_float() / 2 );

	auto response = manager_3D.state.handle_user_input(	manager_3D, 
														event,		
														sampled_tag	);
	update_grab(PhysimosGrab::SCENE, response);
}



bool Physimos::
try_send_event_to_grabbed(InputEvent & event)
{
	switch (grab.state)
	{

	case PhysimosGrab::PUI_MOUSE_GRAB:
		send_event_pui(event);
		break;
	
	case PhysimosGrab::QUAD_MOUSE_GRAB:
		if(manager_3D.manager_q.try_release_quad(event, sampled_tag))
		{
			update_grab(PhysimosGrab::QUAD, InputResponse::MOUSE_RELEASE);
			return false;
		}
		send_event_quad(event);
		break;

	case PhysimosGrab::SCENE_MOUSE_GRAB:
	case PhysimosGrab::SCENE_MOUSE_PAN:
		send_event_scene(event);
		break;

	case PhysimosGrab::NONE:
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

			if(pui.contains_point(auxwin.get_cursor_pos().pixels))
				send_event_pui(event);
			else if(manager_3D.manager_q.try_new_quad_grab(event, sampled_tag))
				update_grab(PhysimosGrab::QUAD, InputResponse::MOUSE_GRAB);
			else
				send_event_scene(event);

			continue;
		}

		if(event.is_keystroke())
		{
			// Global UI shortcuts
			{
				if(event.keystroke.is_press() && event.modifiers.is_alt_ctrl() && event.keystroke.key == Key::a)
					pui.do_action(UI::Actions::ToggleLeftPanel);
				if(event.keystroke.is_press() && event.modifiers.is_alt_ctrl() && event.keystroke.key == Key::d)
					pui.do_action(UI::Actions::ToggleRightPanel);
			}

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
		// pui.event_window_resize(window_event);
		renderer_pui.set_window_info(
			window_event.size_f, 
			window_event.content_scale
		);

		manager_3D.state.handle_window(	manager_3D,
										window_event);

		manager_3D.renderer_3D.set_window_fb_size(window_event);
	}
}



void Physimos::
update()
{
	manager_3D.update(auxwin.timer.get_s());
	pui.reload(	manager_3D, 
				auxwin.get_window_fb_size().to_f2()	);
	// auxwin.get_window_fb_size().to_f2().print("FB size: ");
	
}



void Physimos::
render()
{
	// Rend::Manager::get_renderer_scene2D().render_all_scene2D_to_frambuffers(manager_2D);
	rendererScene2D.render_all_scene2D_to_frambuffers(manager_2D);

	manager_3D.render_window_scene(	auxwin.get_window_fb_size(),
									auxwin.get_cursor_pos()	);


	f2 sample_pos;
	if(grab.is_scene_mouse_grab())
		sample_pos = auxwin.get_center_pos();
	else if(grab.is_scene_mouse_pan())
		sample_pos = {-10.0f, -10.0f}; // Make sure no sucessful sampling takes place
	else
		sample_pos = auxwin.get_cursor_pos().pixels;

	sampled_tag = manager_3D.renderer_3D.sample_oid_tag(  	*manager_3D.window_scene,
                                                			sample_pos    					);

	pui.render(renderer_pui); // Render ui on top of scene
}



void Physimos::
main_loop()
{
	auxwin.timer.start(); // make sure timer has been started before first dt is calculated to prevent unix-time-sized deltas

	while (auxwin.is_open())
	{
		auxwin.new_frame();
		// auxwin.timer.print_duration("dt: ");
		// auxwin.timer.print_ms("dt: ");
		// auxwin.timer.print_s("dt: ");
		// auxwin.print_fps();
		


		process_framebuffer_events(); // change framebuffer size, content scale
		process_user_input();

		update();

		render();
		
		auxwin.end_frame();
	}

	auxwin.destroy();
}





