

#include "auxwin.hh"
// #include "opengl/program.hh"

#include "phont.hh"



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;



int main()
{
	// WINDOW
	Auxwin auxwin(WINDOW_WIDTH, WINDOW_HEIGHT);


    // QUAD
	phont::Quad quad = phont::Quad( (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT );


    // TEXTURE
    unsigned int texture_checker = 0;
    phont::set_texture_checker(texture_checker);
	unsigned int texture_F = phont::get_texture_F();


	// RENDERER
	phont::QuadRenderer renderer_quad;
	renderer_quad.create_context(quad);

	quad.render_context.texture = texture_checker;
	quad.render_context.texture = texture_F;



	while (auxwin.is_open())
	{
		auxwin.new_frame();


		// INPUT
		InputState input_state = auxwin.handle_input();
		if(input_state.esc){
			auxwin.close();
		}


		// RENDER
		renderer_quad.render_context(quad.render_context);


		auxwin.end_frame();
	}

	quad.render_context.delete_contents();


	// glDeleteProgram(program); // NOTE: This was never replaced!
	// glfwTerminate();
	auxwin.destroy();

	return 0;
}




