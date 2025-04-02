#include <iostream>
#include <queue>

#include "window/auxwin.hh"

#include "phont.hh"



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

scene::Scene2D scene_2d;

bool mouse_pan = false;

int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	// WINDOW
	window::Auxwin auxwin(WINDOW_WIDTH, WINDOW_HEIGHT);


    // QUAD
	opengl::Quad2D quad = opengl::Quad2D( (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT );


    // TEXTURE
    unsigned int texture_checker = 0;
    phont::set_texture_checker(texture_checker);
	opengl::TextureFrameBuffer texture_framebuffer_F = phont::get_texture_F();


	// RENDERER
	opengl::QuadRenderer renderer_quad;
	renderer_quad.create_context(quad);
	
	// quad.transform_2d.pos = {0.5f, 0.5f};
	// quad.transform_2d.scale = {2.0f, 2.0f};
	quad.transform_2d.set_pos(0.5f, 0.5f);
	quad.transform_2d.set_scale(2.0f, 2.0f);
	quad.transform_2d.set_matrix_model();

	scene_2d.camera_2d.transform_2d.pos = {0.0f, 0.0f};
	scene_2d.camera_2d.transform_2d.set_pos	 (0.5f, 0.5f);
	scene_2d.camera_2d.transform_2d.set_scale(1.0f, 1.0f);
	scene_2d.camera_2d.transform_2d.set_matrix_camera();
	
	renderer_quad.set_model_camera(quad.transform_2d.matrix, scene_2d.camera_2d.transform_2d.matrix);


	// quad.render_context.texture = texture_checker;
	quad.render_context.texture = texture_framebuffer_F.texture_id;
	
	

	while (auxwin.is_open())
	{
		auxwin.new_frame();


		// INPUT
		std::queue<window::InputEvent> input_events = auxwin.get_input_events();
		// std::cout << "input_events.size()" << input_events.size() << std::endl;
		
		while(input_events.size() > 0){
			window::InputEvent event = input_events.front();
			input_events.pop();


			if(event.type == window::EventType::MouseButton){

				// PAN
				if(event.mouse_button.button == window::MouseButton::Middle) {
			
					if(event.mouse_button.action == window::ButtonAction::Press)
						mouse_pan = true;
					else if(event.mouse_button.action == window::ButtonAction::Release)
						mouse_pan = false;

				}
			
			}
			else if(event.type == window::EventType::MouseScroll){
				float zoom_multiplier = 1.1f;
				if(event.mouse_scroll.delta > 0)
					scene_2d.camera_2d.zoom(zoom_multiplier);
				else 
					scene_2d.camera_2d.zoom(1 / zoom_multiplier);

				scene_2d.camera_2d.transform_2d.set_matrix_camera();

				// scene_2d.camera_2d.transform_2d.
				// // PAN
				// if(event.mouse_button.button == window::MouseButton::Middle) {
			
				// 	if(event.mouse_button.action == window::ButtonAction::Press)
				// 		mouse_pan = true;
				// 	else if(event.mouse_button.action == window::ButtonAction::Release)
				// 		mouse_pan = false;
						
				// }
			
			}
			if(event.key_stroke.key == window::Key::Esc)
				auxwin.close();

			
			if(event.type == window::EventType::MouseMove){
				if(mouse_pan){
					float pan_scale = 0.01f;
					scene_2d.camera_2d.transform_2d.move_x(- event.mouse_movement.delta.x * pan_scale);
					scene_2d.camera_2d.transform_2d.move_y(- event.mouse_movement.delta.y * pan_scale);
					scene_2d.camera_2d.transform_2d.set_matrix_camera();
				}
			}


			if(event.key_stroke.key == window::Key::p && event.key_stroke.action == window::ButtonAction::Press){
				quad.transform_2d.matrix.print();
				// scene_2d.camera_2d.transform_2d.matrix.print();
				std::cout << "mouse_pan = " << mouse_pan << std::endl;
				
			}
		}


		// RENDER
		renderer_quad.set_model_camera(quad.transform_2d.matrix, scene_2d.camera_2d.transform_2d.matrix);
		renderer_quad.render(quad.render_context);


		auxwin.end_frame();
	}

	quad.render_context.delete_contents();


	// glDeleteProgram(program); // NOTE: This was never replaced!
	// glfwTerminate();
	auxwin.destroy();

	return 0;
}




