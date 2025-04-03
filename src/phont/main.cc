#include <iostream>
#include <queue>

#include "window/auxwin.hh"

#include "scene/scene_2d.hh"

#include "phont.hh"



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const float WINDOW_WIDTH_F = 800.0f;
const float WINDOW_HEIGHT_F = 600.0f;
f2 window_size = {800.0f, 600.0f};

float aspect_ratio = WINDOW_WIDTH_F / WINDOW_HEIGHT_F;


int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	// WINDOW
	window::Auxwin auxwin(WINDOW_WIDTH, WINDOW_HEIGHT);

	// SCENE 2D
	scene::Scene2D scene = scene::Scene2D(window_size);
	scene.set_window_size({ WINDOW_WIDTH_F, WINDOW_HEIGHT_F} );

    // TEXTURE (UNUSED)
    unsigned int texture_checker = 0;
    phont::set_texture_checker(texture_checker);
	
	

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

			scene.handle_input(event);
		}

		// UPDATE
		scene.update();

		// RENDER
		scene.render();


		auxwin.end_frame();
	}

	scene.quad.render_context.delete_contents();


	// glDeleteProgram(program); // NOTE: This was never replaced!
	// glfwTerminate();
	auxwin.destroy();

	return 0;
}




