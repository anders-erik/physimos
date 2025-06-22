
#include "rend/rend_manager.hh"

#include "conductor2D.hh"



Physimos::Physimos(int width, int height) 
{
	auxwin.init(width, height);

	Rend::Manager::init( {width, height} );

	// First scene added to scene manager becomes root
	Scene3D& root_scene = ManagerScene::init( {width, height} );

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
		Object hovered_object = rend_scene3D.sample_object_in_fb(event.cursor_pos.sane);

		Scene3D& root_scene = ManagerScene::get_root_scene_mut();

		if(hovered_object.id != root_scene.capturing_quad.id)
			return true;	
	}

	return false;
}






void Physimos::
send_event_scene(InputEvent& event)
{
	auto response = ManagerScene::event_send_window_scene(event);
	input_state.update(GrabStateConductor::SCENE, response);

	pui.clear_hovers();
}



void Physimos::
send_event_quad(InputEvent& event)
{
	Scene3D& window_scene = ManagerScene::get_window_scene_mut();
	auto& capturing_object = window_scene.capturing_quad;
	auto* capturing_quado = ObjectManager::get_quado(capturing_object.id);
	if(capturing_quado == nullptr)
	{
		window_scene.capturing_quad = {};
		input_state.clear();
		return;
	}

	// FINALLY SEND INFO
	if(capturing_quado->texture.is_scene2D())
	{
		std::cout << "Sending to scene" << std::endl;
		Scene2D* scene_p = ManagerScene::get_manager_2D().search_scene_storage_2D(capturing_quado->texture.sid);
		
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
	window_scene.capturing_quad = {};
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
			auto response = pui.event_all(event);
			input_state.update(GrabStateConductor::PUI, response);
			return;
		}
		else if(input_state.quad())
		{
			if(should_release_quad(event))
			{	
				ManagerScene::get_root_scene_mut().capturing_quad = {};
				input_state.update(GrabStateConductor::QUAD, {InputResponse::MOUSE_RELEASE});
				return;
			}
			send_event_quad(event);
			return;
		}
		else if(input_state.scene())
		{
			send_event_scene(event);
			return;
		}
	}


	// Requery if cursor is not grabbed
	if(pui.contains_point(cursor_pos.sane))
	{
		auto response = pui.event_all(event);
		input_state.update(GrabStateConductor::PUI, response);
		return;
	}

	// Scene OR quad at this point

	auto& rend_scene3D = Rend::Manager::get_renderer_scene3D();
	Object hovered_object = rend_scene3D.sample_object_in_fb(event.cursor_pos.sane);

	if(hovered_object.is_quad() && is_quad_capture_click(event))
	{
		std::cout << "conductor :: Capturing Quad state set" << std::endl;
		ManagerScene::get_window_scene_mut().capturing_quad = hovered_object;
		input_state.update(GrabStateConductor::QUAD, {InputResponse::MOUSE_GRAB});
		return;
	}
	else
	{
		ManagerScene::get_window_scene_mut().hovered_object = hovered_object;
		send_event_scene(event);
		return;
	}
}


void Physimos::
process_keyboard_input(InputEvent & event)
{
	std::cout << "KEYSTROKE" << std::endl;
			
	if(input_state.quad())
	{
		if(event.keystroke.key == Key::Esc)
		{
			Scene3D& window_scene = ManagerScene::get_window_scene_mut();
			window_scene.capturing_quad = {};
			input_state.clear();
			return;
		}
	}

	ManagerScene::event_send_window_scene(event);
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
		ManagerScene::event_window_resize(window_event);

		Rend::Manager::get_renderer_scene3D().set_window_fb_size(window_event);
	}
}



void Physimos::
update()
{
	pui.update(); // reflect scene state changes
}



void Physimos::
render()
{
	Rend::Manager::get_renderer_scene2D().render_all_scene2D_to_frambuffers();

	auxwin.bind_window_framebuffer();

	Scene3D& window_scene = ManagerScene::get_window_scene_mut();

	Scene3D& window_scene_3D = (Scene3D&) window_scene;

	auto& rend_scene3D = Rend::Manager::get_renderer_scene3D();
	rend_scene3D.render_scene_3d(window_scene_3D);
	rend_scene3D.render_object_ids(window_scene_3D);
	

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





