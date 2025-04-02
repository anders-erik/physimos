#include <iostream>
#include <queue>

#include "window/auxwin.hh"

#include "phont.hh"



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;



int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	// WINDOW
	window::Auxwin auxwin(WINDOW_WIDTH, WINDOW_HEIGHT);


    // QUAD
	phont::Quad quad = phont::Quad( (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT );


    // TEXTURE
    unsigned int texture_checker = 0;
    phont::set_texture_checker(texture_checker);
	opengl::TextureFrameBuffer texture_framebuffer_F = phont::get_texture_F();


	// RENDERER
	phont::QuadRenderer renderer_quad;
	renderer_quad.create_context(quad);

	// quad.render_context.texture = texture_checker;
	quad.render_context.texture = texture_framebuffer_F.texture_id;
	
	

	while (auxwin.is_open())
	{
		auxwin.new_frame();


		// INPUT
		std::queue<window::InputEvent> input_events = auxwin.get_input_events();

		while(input_events.size() > 0){
			window::InputEvent event = input_events.front();
			input_events.pop();

			if(event.key_stroke.key == window::Key::Esc)
				auxwin.close();


		}


		// RENDER
		renderer_quad.render(quad.render_context);


		auxwin.end_frame();
	}

	quad.render_context.delete_contents();


	// glDeleteProgram(program); // NOTE: This was never replaced!
	// glfwTerminate();
	auxwin.destroy();

	return 0;
}




