#include <stdexcept>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <any>


#include "manager.hh"

#include "conductor/conductor2D.hh"

#include "phont.hh"



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

f2 window_size = {800.0f, 600.0f};

phont::Face face;

InputEvent event;


// void add_glyph_quads_to_scene(scene::Scene2D& scene){
void add_glyph_quads_to_scene(scene::Scene2D* scene){


	// Glyph F 1
	phont::Glyph glyph_F_1 {'F', {10, 16}};
	glyph_F_1.generate_multisample();

	scene::QuadS2D quad_F_1;
	quad_F_1.set_box({3.0f, 3.0f}, {2.0f, 3.0f});
	quad_F_1.set_texture_id(glyph_F_1.text_framebuff_multi.resolvedTexture);
	quad_F_1.set_name("quad_F_1");



	phont::Glyph F_10_16 { phont::Glyph( 'F', {10, 16} ) };
	F_10_16.generate_multisample();
	F_10_16.text_framebuff.texture.draw_rect({1, 1}, {2, 2}, {255, 0, 255, 255});

	scene::QuadS2D quad_F_2;
	quad_F_2.set_box( {0.0f, 0.0f}, {2.0f, 3.0f} );
    quad_F_2.set_texture_id(F_10_16.text_framebuff_multi.resolvedTexture);
	quad_F_2.set_name("quad_F_2");




	phont::Glyph A_200_320 { phont::Glyph( 'A', {20, 32} ) };
	A_200_320.generate();

	scene::QuadS2D quad_A_1;
	quad_A_1.set_box( {2.0f, 0.0f}, {2.0f, 3.0f} );
	quad_A_1.set_texture_id(A_200_320.get_texture().id_gl);
	quad_A_1.set_name("quad_A_1");



	phont::Glyph A_multi { phont::Glyph( 'A', {10, 16} ) };;
	A_multi.generate_multisample();

	scene::QuadS2D quad_A_multi;
	quad_A_multi.set_texture_id(A_multi.text_framebuff_multi.resolvedTexture);
	quad_A_multi.set_box( {6.0f, 0.0f}, {2.0f, 3.0f} );
	quad_A_multi.set_name("quad_A_multi");



	// TRIANGLE MULTISAMPLE TESTS

	opengl::TextureFrameBufferMultisample Triangle_text_buf_multi { opengl::TextureFrameBufferMultisample( {20, 32}, 4 ) };
	scene::QuadS2D quad_Triangle;

	// renderer2D.create_context(quad_Triangle);
	// TODO: Render triangle texture
	// quad_Triangle.render_context.texture = A_200_320.get_texture().id_gl; 
	// quad_Triangle.render_context.texture = Triangle_text_buf_multi.text_id;
	// quad_Triangle.render_context.texture = Triangle_text_buf_multi.resolvedTexture;
	quad_Triangle.set_texture_id(Triangle_text_buf_multi.resolvedTexture);
	// opengl::set_texture_checker_2x2(quad_Triangle.render_context.texture);

	Triangle_text_buf_multi.clear_color = {0.5f, 0.0f, 0.5f, 1.0f};
	Triangle_text_buf_multi.multisample_fbo_bind();
	Triangle_text_buf_multi.multisample_fbo_clear();

	Triangle_text_buf_multi.blit();
	Triangle_text_buf_multi.multisample_fbo_unbind();

	// unsigned char *pixel = opengl::Texture::get_pixel({2, 2}, quad_Triangle.render_context.texture);

	quad_Triangle.set_box({4.0f, 0.0f}, {2.0f, 3.0f});
	quad_Triangle.set_name("quad_Triangle");


	

	scene->add_quad(quad_F_1);
	scene->add_quad(quad_F_2);
	scene->add_quad(quad_A_1);
	scene->add_quad(quad_A_multi);
	scene->add_quad(quad_Triangle);

}







int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	f2 conductor_window_size = {800.0f, 600.0f};

	Conductor2D conductor2D (conductor_window_size);


	// ROOT SCENE
	scene::Scene2D* root_scene = ManagerScene::get_root_scene();
	root_scene->name = "Forever Root";
	// ADD GREEN QUADS TO ROOT SCENE
	scene::QuadS2D root_scene_quad;
	root_scene_quad.set_box( {0.0f, 0.0f}, {30.0f, 30.0f} );
    root_scene_quad.set_bitmap_texture(opengl::texture_get_id(opengl::Textures::Grass));
	root_scene_quad.set_name("root_scene_quad_1");
	root_scene->add_quad(root_scene_quad);
	root_scene_quad.set_box( {60.0f, 0.0f}, {30.0f, 30.0f} );
	root_scene_quad.set_name("quad_2");
	root_scene->add_quad(root_scene_quad);



	// SHAPES
	Shape point_to_draw = Shape::create(shape_t::point);
	point_to_draw.move(f2{8.0f, 4.0f});
	Shape line_to_draw = Shape::create(shape_t::line);



	// NEW SUBSCENE ADDED TO ROOT SCENE
	// scene::SubScene2D& subscene_1 = root_scene->add_subscene({450, 300}, {320, 240});
	scene::SubScene2D& subscene_1 = root_scene->add_subscene({500, 200}, {240, 300});
	scene::Scene2D* subscene_1_scene = ManagerScene::get_scene(subscene_1.scene_id);
	subscene_1_scene->name = "Glyph_scene";

	// ADD SHAPES TO SUBSCENE
	subscene_1_scene->add_shape(point_to_draw);
	scene::ShapeS2D& subscene_1_line = subscene_1_scene->add_shape(line_to_draw);
	subscene_1_line.set_pos( {6.0f, 6.0f} );

	// ADD GLYPHS TO SUBSCENE
	add_glyph_quads_to_scene(subscene_1_scene);



	// QUAD SUBSCENE
	scene::Scene2D * quad_subscene = ManagerScene::new_scene({100, 100});
	quad_subscene->name = "100x100 test scene";
	size_t quad_subscene_id = root_scene->add_subscene(quad_subscene);


	// QUAD Glyph Subscene
	scene::Scene2D* glyphscene = ManagerScene::new_scene({240, 300});
	quad_subscene->name = "glyph scene";
	size_t glyphscene_quad_id = root_scene->add_subscene(glyphscene);
	scene::QuadS2D* glyphscene_quad = root_scene->quad_manager.get_quad(glyphscene_quad_id);
	glyphscene_quad->set_name("glyph scene quad");
	glyphscene_quad->set_box({250, 200}, {240, 300});

	// Shapes
	glyphscene->add_shape(point_to_draw);
	scene::ShapeS2D& glyphscene_line = glyphscene->add_shape(line_to_draw);
	glyphscene_line.set_pos( {6.0f, 6.0f} );
	// Glyphs
	add_glyph_quads_to_scene(glyphscene);



	



	conductor2D.main_loop();

	return 0;
}




