#include <iostream>
#include <queue>
#include <map>

#include "window/auxwin.hh"

#include "scene/scene_2d.hh"

#include "phont.hh"



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

f2 window_size = {800.0f, 600.0f};

phont::Face face;


int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	// WINDOW
	window::Auxwin auxwin(WINDOW_WIDTH, WINDOW_HEIGHT);

	// OPENGL
	opengl::build_program_vert_frag(opengl::Programs::ndc_black);

	// Glyph F 1
	phont::Glyph glyph_F_1 {'F', {10, 16}};
	glyph_F_1.generate_multisample();

	opengl::Quad2D quad_F_1;
	quad_F_1.transform_2d.set_pos(3.0f, 3.0f);
	quad_F_1.transform_2d.set_scale(2.0f, 3.0f);
    // quad_F.render_context.texture = F_10_16.get_texture().id_gl;
    quad_F_1.render_context.texture = glyph_F_1.text_framebuff_multi.resolvedTexture;

	// SCENE 2D
	scene2D::Scene2D scene = scene2D::Scene2D(window_size);
	scene.add_quad(quad_F_1);
	

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

	scene.quad_F.render_context.delete_contents();


	// glDeleteProgram(program); // NOTE: This was never replaced!
	// glfwTerminate();
	auxwin.destroy();

	return 0;
}




