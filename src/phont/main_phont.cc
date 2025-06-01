#include <stdexcept>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <any>


#include "window/win2D.hh"

#include "phont.hh"



const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

f2 window_size = {800.0f, 600.0f};

phont::Face face;

InputEvent event;


// void add_glyph_quads_to_scene(scene::Scene2D& scene){
void add_glyph_quads_to_scene(scene::SubScene2D& subscene){


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



	phont::Glyph A_multi { phont::Glyph( 'A', {15, 24} ) };;
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



	subscene.add_quad(quad_F_1);
	subscene.add_quad(quad_F_2);
	subscene.add_quad(quad_A_1);
	subscene.add_quad(quad_A_multi);
	subscene.add_quad(quad_Triangle);

}



// Borrow checker (runtime)
class BC {

	std::map<unsigned int, std::any> scene_map;
	unsigned int scene_id_count = 0;
	bool scene_is_locked = false;

public:
	enum Type {
		scene,
	};
	struct Tag {
		unsigned int id;
		Type type;
		std::string label;

		Tag(unsigned int id, Type type,std::string label) : id {id}, type {type}, label{label} {};
	};


	Tag new_scene(std::string label){
		scene_map[scene_id_count++] = scene::Scene2D {{0,0}};
		return Tag{scene_id_count-1, Type::scene, label};
	}

	scene::Scene2D& borrow_scene(Tag tag){
		scene::Scene2D& scene = std::any_cast<scene::Scene2D&>(scene_map[tag.id]); 

		if(scene_is_locked)
			throw std::runtime_error("BC: Tried to get scene during scene lock.");

		scene_is_locked = true;

		return scene;
	}

	void return_scene(Tag tag){

		scene_is_locked = false;
	}

};




int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	Win2D win2D ({800, 600});

	// scene::Scene2D& root_scene_0 = win2D.add_subscene({0.1f, 0.4f}, {0.5f, 0.5f});
	scene::SubScene2D& subscene_0 = win2D.add_subscene({0.1f, 0.4f}, {0.5f, 0.5f});

	// Add stuff to scene

	Shape point_to_draw = Shape::create(shape_t::point);
	point_to_draw.move(f2{8.0f, 4.0f});
	scene::ShapeS2D& scene_point = subscene_0.add_shape(point_to_draw);
	// scene_point.

	Shape line_to_draw = Shape::create(shape_t::line);
	// line_to_draw.move(f2{6.0f, 6.0f});
	scene::ShapeS2D& scene_line = subscene_0.add_shape(line_to_draw);
	scene_line.set_pos( {6.0f, 6.0f} );


	add_glyph_quads_to_scene(subscene_0);



	BC bc;

	BC::Tag tag_scene_0 = bc.new_scene("Scene0");
	scene::Scene2D& borrowed_scene_0 = bc.borrow_scene(tag_scene_0);
	// scene::Scene2D& borrowed_scene_0_again = bc.borrow_scene(tag_scene_0); // not returned!
	// Change value
	borrowed_scene_0.get_window_size().print("Initial window size: ");
	borrowed_scene_0.set_window_size({100, 100});
	bc.return_scene(tag_scene_0);

	// Read value change
	scene::Scene2D& borrowed_scene_0_again = bc.borrow_scene(tag_scene_0);
	borrowed_scene_0_again.get_window_size().print("Updated window size: ");
	bc.return_scene(tag_scene_0);
	


	win2D.start_loop();

	return 0;
}




