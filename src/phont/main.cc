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

window::InputEvent event;

int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	// WINDOW
	window::Auxwin auxwin;
	auxwin.init(WINDOW_WIDTH, WINDOW_HEIGHT);

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


    opengl::Quad2D quad_F_2;
	phont::Glyph F_10_16 { phont::Glyph( 'F', {10, 16} ) };
    F_10_16.generate_multisample();
    F_10_16.text_framebuff.texture.draw_rect({1, 1}, {2, 2}, {255, 0, 255, 255});

	quad_F_2.transform_2d.set_pos(0.0f, 0.0f);
	quad_F_2.transform_2d.set_scale(2.0f, 3.0f);
    quad_F_2.render_context.texture = F_10_16.text_framebuff_multi.resolvedTexture;


    phont::Glyph A_200_320 { phont::Glyph( 'A', {20, 32} ) };
    opengl::Quad2D quad_A_1;
	
    A_200_320.generate();

    quad_A_1.render_context.texture = A_200_320.get_texture().id_gl;
	quad_A_1.transform_2d.set_pos(2.0f, 0.0f);
	quad_A_1.transform_2d.set_scale(2.0f, 3.0f);



    phont::Glyph A_multi { phont::Glyph( 'A', {15, 24} ) };;
    opengl::Quad2D quad_A_multi;
	
    A_multi.generate_multisample();
    // renderer_quad.create_context(quad_A_multi);
    quad_A_multi.render_context.texture = A_multi.text_framebuff_multi.resolvedTexture;
	quad_A_multi.transform_2d.set_pos(6.0f, 0.0f);
	quad_A_multi.transform_2d.set_scale(2.0f, 3.0f);



	// TRIANGLE MULTISAMPLE

    opengl::TextureFrameBufferMultisample Triangle_text_buf_multi { opengl::TextureFrameBufferMultisample( {20, 32}, 4 ) };
    opengl::Quad2D quad_Triangle;


    // renderer_quad.create_context(quad_Triangle);
    // TODO: Render triangle texture
    // quad_Triangle.render_context.texture = A_200_320.get_texture().id_gl; 
    // quad_Triangle.render_context.texture = Triangle_text_buf_multi.text_id;
    quad_Triangle.render_context.texture = Triangle_text_buf_multi.resolvedTexture;
    // opengl::set_texture_checker_2x2(quad_Triangle.render_context.texture);

    Triangle_text_buf_multi.clear_color = {0.5f, 0.0f, 0.5f, 1.0f};
    Triangle_text_buf_multi.multisample_fbo_bind();
    Triangle_text_buf_multi.multisample_fbo_clear();

    Triangle_text_buf_multi.blit();
    Triangle_text_buf_multi.multisample_fbo_unbind();

    unsigned char *pixel = opengl::Texture::get_pixel({2, 2}, quad_Triangle.render_context.texture);

	quad_Triangle.transform_2d.set_pos(4.0f, 0.0f);
	quad_Triangle.transform_2d.set_scale(2.0f, 3.0f);




	// SCENE 2D
	scene2D::Scene2D scene = scene2D::Scene2D(window_size);
	scene.add_quad(quad_F_1);
	scene.add_quad(quad_F_2);
	scene.add_quad(quad_A_1);
	scene.add_quad(quad_A_multi);
	scene.add_quad(quad_Triangle);
	


	while (auxwin.is_open())
	{
		auxwin.new_frame();


		// Handle Input
		while(event = auxwin.get_input_event(), event.type != window::EventType::None){

            if(event.key_stroke.key == window::Key::Esc)
                auxwin.close();
            
			scene.handle_input(event);
        }

		// UPDATE
		scene.update();

		// RENDER
		scene.render_window();


		auxwin.end_frame();
	}

	// scene.quad_F.render_context.delete_contents();


	// glDeleteProgram(program); // NOTE: This was never replaced!
	// glfwTerminate();
	auxwin.destroy();

	return 0;
}




