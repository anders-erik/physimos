
#include "rend/rend_manager.hh"

#include "conductor2D.hh"



Physimos::Physimos(int width, int height) 
{
	auxwin.init(width, height);

	Rend::Manager::init( {width, height} );

	// First scene added to scene manager becomes root
	manager_3D.init();

}

Scene3D & Physimos::get_window_scene()
{
	return *manager_3D.window_scene;
}




bool Physimos::
is_quad_capture_click(InputEvent & event)
{
	if(event.is_mouse_button() && event.mouse_button.is_left_down() && event.modifiers.is_ctrl())
		return true;

	return false;
}


bool Physimos::
should_release_quad(InputEvent & event)
{
	if(event.is_mouse_button() && event.mouse_button.is_left_down())
	{
		auto& rend_scene3D = Rend::Manager::get_renderer_scene3D();
		Object* hovered_object = rend_scene3D.sample_object_in_fb(event.cursor_pos.sane);
		if(hovered_object == nullptr)
			return true;

		if(hovered_object->tag.oid != manager_3D.state.capturing_quad_tag.oid)
			return true;	
	}

	return false;
}






void Physimos::
send_event_window_scene(InputEvent& event)
{
	auto response = manager_3D.state.handle(	*manager_3D.window_scene, 
												event						);
	input_state.update(GrabStateConductor::SCENE, response);

	pui.clear_hovers();
}



void Physimos::
send_event_quad(InputEvent& event)
{
	auto* capturing_quado = ObjectManager::get_squado(manager_3D.state.capturing_quad_tag);
	if(capturing_quado == nullptr)
	{
		manager_3D.state.capturing_quad_tag = {};
		input_state.clear();
		return;
	}

	// FINALLY SEND INFO
	if(capturing_quado->squad.is_scene2D())
	{
		std::cout << "Sending to scene" << std::endl;
		Scene2D* scene_p = manager_2D.search_scene_storage_2D(capturing_quado->squad.sid);
		
		if(event.is_mouse_scroll())
			scene_p->handle_scroll(event);
		if(event.is_mouse_button())
			scene_p->handle_pointer_click(event);
		if(event.is_mouse_movement())
			scene_p->handle_pointer_move(event);

		input_state.update(GrabStateConductor::QUAD, {InputResponse::MOUSE_GRAB});
		return;
	}

	// always release unless already continued with new grab
	manager_3D.state.capturing_quad_tag = {};
	input_state.clear();
}




void Physimos::
process_mouse_input(InputEvent& event)
{
	// Short Circuit on grab
	if(input_state.is_grabbing_mouse())
	{
		if(input_state.pui())
		{
			auto response = pui.event_all(manager_3D, event);
			input_state.update(GrabStateConductor::PUI, response);
			return;
		}
		else if(input_state.quad())
		{
			if(should_release_quad(event))
			{	
				std::cout << "RELEASE GRABBING QUAD" << std::endl;
				manager_3D.state.capturing_quad_tag = {};
				input_state.update(GrabStateConductor::QUAD, {InputResponse::MOUSE_RELEASE});
				return;
			}
			send_event_quad(event);
			return;
		}
		else if(input_state.scene())
		{
			send_event_window_scene(event);
			return;
		}
	}


	// Requery if cursor is not grabbed
	if(pui.contains_point(cursor_pos.sane))
	{
		auto response = pui.event_all(manager_3D, event);
		input_state.update(GrabStateConductor::PUI, response);
		return;
	}

	// Scene OR quad at this point

	auto& rend_scene3D = Rend::Manager::get_renderer_scene3D();
	Object* hovered_object = rend_scene3D.sample_object_in_fb(event.cursor_pos.sane);


	if(hovered_object != nullptr && hovered_object->tag.is_quad() && is_quad_capture_click(event))
	{
		std::cout << "conductor :: Capturing Quad state set" << std::endl;
		manager_3D.state.capturing_quad_tag = hovered_object->tag;
		input_state.update(GrabStateConductor::QUAD, {InputResponse::MOUSE_GRAB});
		return;
	}
	else
	{
		if(hovered_object == nullptr)
			manager_3D.state.hovered_tag = TagO();
		else
			manager_3D.state.hovered_tag = hovered_object->tag;

		send_event_window_scene(event);
		return;
	}
}


void Physimos::
process_keyboard_input(InputEvent & event)
{			
	if(input_state.quad())
	{
		if(event.keystroke.key == Key::Esc)
		{
			manager_3D.state.capturing_quad_tag = {};
			input_state.clear();
		}
		return;
	}

	send_event_window_scene(event);
}



void Physimos::
process_user_input()
{
	auto events = auxwin.get_events_input();

	for(InputEvent& event : events)
	{

		if(event.is_mouse())
		{
			process_mouse_input(event);

			// Cache most up to date cursor position
			if(event.is_mouse_movement())
				cursor_pos = event.mouse_movement.cursor_new;
			continue;
		}

		if(event.is_keystroke()) // Currently only the scene recieves keyboard input
		{
			process_keyboard_input(event);
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

		manager_3D.state.handle_window(	*manager_3D.window_scene,
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
	rend_scene3D.render_scene_3d(*manager_3D.window_scene, manager_3D.state);
	rend_scene3D.render_object_ids(*manager_3D.window_scene);
	
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





