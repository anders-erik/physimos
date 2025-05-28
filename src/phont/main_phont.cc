#include <iostream>
#include <queue>
#include <map>

#include "window/auxwin.hh"

#include "scene/scene2D.hh"
#include "scene/shapeS2D.hh"

#include "phont.hh"

using namespace window;


const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

f2 window_size = {800.0f, 600.0f};

phont::Face face;

InputEvent event;


void add_glyph_quads_to_scene(scene::Scene2D& scene){


	// Glyph F 1
	phont::Glyph glyph_F_1 {'F', {10, 16}};
	glyph_F_1.generate_multisample();

	scene::ShapeS2D quad_F_1;
	quad_F_1.transform_2d.set_pos(3.0f, 3.0f);
	quad_F_1.transform_2d.set_scale(2.0f, 3.0f);
	// quad_F.render_context.texture = F_10_16.get_texture().id_gl;
	quad_F_1.render_context.texture = glyph_F_1.text_framebuff_multi.resolvedTexture;


    	scene::ShapeS2D quad_F_2;
	phont::Glyph F_10_16 { phont::Glyph( 'F', {10, 16} ) };
	F_10_16.generate_multisample();
	F_10_16.text_framebuff.texture.draw_rect({1, 1}, {2, 2}, {255, 0, 255, 255});

	quad_F_2.transform_2d.set_pos(0.0f, 0.0f);
	quad_F_2.transform_2d.set_scale(2.0f, 3.0f);
    	quad_F_2.render_context.texture = F_10_16.text_framebuff_multi.resolvedTexture;


        phont::Glyph A_200_320 { phont::Glyph( 'A', {20, 32} ) };
	scene::ShapeS2D quad_A_1;
		
        A_200_320.generate();

	quad_A_1.render_context.texture = A_200_320.get_texture().id_gl;
	quad_A_1.transform_2d.set_pos(2.0f, 0.0f);
	quad_A_1.transform_2d.set_scale(2.0f, 3.0f);



	phont::Glyph A_multi { phont::Glyph( 'A', {15, 24} ) };;
	scene::ShapeS2D quad_A_multi;
		
	A_multi.generate_multisample();
	// renderer2D.create_context(quad_A_multi);
	quad_A_multi.render_context.texture = A_multi.text_framebuff_multi.resolvedTexture;
	quad_A_multi.transform_2d.set_pos(6.0f, 0.0f);
	quad_A_multi.transform_2d.set_scale(2.0f, 3.0f);



	// TRIANGLE MULTISAMPLE

        opengl::TextureFrameBufferMultisample Triangle_text_buf_multi { opengl::TextureFrameBufferMultisample( {20, 32}, 4 ) };
        scene::ShapeS2D quad_Triangle;


        // renderer2D.create_context(quad_Triangle);
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

        // unsigned char *pixel = opengl::Texture::get_pixel({2, 2}, quad_Triangle.render_context.texture);

	quad_Triangle.transform_2d.set_pos(4.0f, 0.0f);
	quad_Triangle.transform_2d.set_scale(2.0f, 3.0f);


	scene.add_quad(quad_F_1);
	scene.add_quad(quad_F_2);
	scene.add_quad(quad_A_1);
	scene.add_quad(quad_A_multi);
	scene.add_quad(quad_Triangle);

}




/** 
	Owns a window and root 2D scenes.
	Handles events and decides which root scene will recieve input events.
*/
class Win2D {
	window::Auxwin auxwin;

	scene::Camera2D camera_root; // Always displays the root scene filling the window
	opengl::Scene2DRenderer renderer;
	// std::vector<scene::ShapeS2D> quads;
	scene::QuadS2D quad0;

	f2 window_size_f;
	std::vector<scene::Scene2D> scenes; // Root scenes owned by Win2D
	std::vector<window::InputEvent> input_events;

public:

	Win2D(f2 window_size);

	scene::Scene2D& add_subscene(f2 pos_normalized, f2 size_normalized);
	void set_window_size(f2 size);

	// launch window and transfer control
	void open();
        void start_frame();

	void process_input(InputEvent & event);

private:
	void reload_camera_root();
	void render_root();
};






Win2D::Win2D(f2 window_size_f) 
	: auxwin {window_size_f} 
{

	this->window_size_f = window_size_f;

	reload_camera_root();

	opengl::build_program_vert_frag(opengl::Programs::ndc_black);


}


scene::Scene2D& Win2D::add_subscene(f2 pos_normalized, f2 size_normalized){

	scene::Scene2D& scene = scenes.emplace_back(window_size_f);
	scene.set_window_norm_box(pos_normalized, size_normalized);

	// quad0.set_dims({10.0f, 10.0f}, {100.0f, 100.0f});
	quad0.set_dims({50.0f, 400.0f}, {window_size_f.x/5.0f, window_size_f.y/5.0f});
	renderer.create_context_quad_t(quad0.get_rendering_context(), quad0.get_verts());

	return scene;
}


void Win2D::set_window_size(f2 size){
	for(scene::Scene2D& scene : scenes)
		scene.set_window_size(size);
}


void Win2D::open(){

	while (auxwin.end_frame()){
		start_frame();

		scene::Scene2D& scene = scenes[0];

		// UPDATE
		scene.update();

		// RENDER
		quad0.set_texture_id(scene.render_texture());

		auxwin.bind_framebuffer();
		// scene.set_window_size(window_size_f);
		// scene.render_window();

		
		
		render_root();
	}

	auxwin.destroy();
}


void Win2D::process_input(InputEvent& event){

        // Only one scene at the moment
        scene::Scene2D& scene = scenes[0];

        switch (event.type){

        case EventType::WindowResize:
				window_size_f = event.window_resize.size_f;
				reload_camera_root();
                // scene.set_window_size(event.window_resize.size_f);
                break;

        case EventType::MouseMove:
                scene.handle_input(event);
                break;
        
        default:
                // std::cout << "WARN: unhandled input event" << std::endl;
                scene.handle_input(event);
                break;
        }

}


void Win2D::start_frame(){

	input_events = auxwin.new_frame();

        for(InputEvent& event : input_events)
                process_input(event);

}

void Win2D::reload_camera_root(){
	renderer.activate();
	camera_root.set_window_size_px(window_size_f);
	camera_root.set_width(window_size_f.x);
	renderer.set_camera(camera_root.get_matrix());
}
void Win2D::render_root(){
	auxwin.bind_framebuffer();
	renderer.activate();
	renderer.set_model(quad0.get_matrix());
	renderer.render_quad(quad0.get_rendering_context());
}


int main()
{
	std::cout << "PHONT MAIN"  << std::endl;

	Win2D win2D ({800, 600});

	scene::Scene2D& root_scene_0 = win2D.add_subscene({0.05f, 0.05f}, {0.9f, 0.9f});

	// Add stuff to scene

	Shape point_to_draw = Shape::create(shape_t::point);
	point_to_draw.move(f2{8.0f, 4.0f});
	root_scene_0.add_shape(point_to_draw);

	add_glyph_quads_to_scene(root_scene_0);


	
	win2D.open();

	return 0;
}




