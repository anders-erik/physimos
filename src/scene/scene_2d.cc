#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "scene/scene_2d.hh"
#include "math/transform.hh"

namespace scene2D {


void CursorContext2D::set_cursor_pos(f2 pos_px, f2 pos_norm, Box2D camera_box){
    viewport_px = pos_px;
    viewport_norm = pos_norm;

    scene.x = camera_box.pos.x + (viewport_norm.x - 0.5) * camera_box.size.x ;
    scene.y = camera_box.pos.y + (viewport_norm.y - 0.5) * camera_box.size.y ;
}




Scene2D::Scene2D(f2 _window_size)
    :   F_10_16 { phont::Glyph( 'F', {10, 16} ) },
        A_200_320 { phont::Glyph( 'A', {20, 32} ) },
        // A_multi { phont::GlyphTextureGenerator( 'A', {150, 240} ) },
        A_multi { phont::Glyph( 'A', {15, 24} ) },
        Triangle_text_buf_multi { opengl::TextureFrameBufferMultisample( {20, 32}, 4 ) }
{
    set_window_size(_window_size);

    // F_10_16.generate();
    F_10_16.generate_multisample();
    F_10_16.text_framebuff.texture.draw_rect({1, 1}, {2, 2}, {255, 0, 255, 255});

    renderer_quad.create_context(quad_F);
	quad_F.transform_2d.set_pos(0.0f, 0.0f);
	quad_F.transform_2d.set_scale(2.0f, 3.0f);
    // quad_F.render_context.texture = F_10_16.get_texture().id_gl;
    quad_F.render_context.texture = F_10_16.text_framebuff_multi.resolvedTexture;



    A_200_320.generate();

    renderer_quad.create_context(quad_A);
    quad_A.render_context.texture = A_200_320.get_texture().id_gl;
	quad_A.transform_2d.set_pos(2.0f, 0.0f);
	quad_A.transform_2d.set_scale(2.0f, 3.0f);


    A_multi.generate_multisample();
    renderer_quad.create_context(quad_A_multi);
    quad_A_multi.render_context.texture = A_multi.text_framebuff_multi.resolvedTexture;
	quad_A_multi.transform_2d.set_pos(6.0f, 0.0f);
	quad_A_multi.transform_2d.set_scale(2.0f, 3.0f);


    // TRIANGLE MULTISAMPLE

    renderer_quad.create_context(quad_Triangle);
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


    camera.set_window_size_px(window_size);
    camera.zoom_set(0.2f);
    camera.set_zoom_multiplier(1.2f);
    camera.pan({ -400.0f, -300.0f}); // Half of window size

}


void Scene2D::set_window_size(f2 size){
    window_size = size;

    camera.set_window_size_px(size);
}


void Scene2D::add_quad(opengl::Quad2D& quad_){
    renderer_quad.create_context(quad_);
    quads.push_back(quad_);
}

void Scene2D::update(){

    quad_F.transform_2d.set_matrix_model();
    quad_A.transform_2d.set_matrix_model();
    quad_A_multi.transform_2d.set_matrix_model();
    quad_Triangle.transform_2d.set_matrix_model();

    for(opengl::Quad2D& _quad : quads)
        _quad.transform_2d.set_matrix_model();

    camera.transform.set_matrix_camera();

    // renderer_quad.set_model_camera(quad_F.transform_2d.matrix, camera.transform.matrix);
}


void Scene2D::render(){
    // RENDER
    renderer_quad.set_model_camera(quad_F.transform_2d.matrix, camera.transform.matrix);
    renderer_quad.render(quad_F.render_context);

    renderer_quad.set_model_camera(quad_A.transform_2d.matrix, camera.transform.matrix);
    renderer_quad.render(quad_A.render_context);

    renderer_quad.set_model_camera(quad_A_multi.transform_2d.matrix, camera.transform.matrix);
    renderer_quad.render(quad_A_multi.render_context);

    renderer_quad.set_model_camera(quad_Triangle.transform_2d.matrix, camera.transform.matrix);
    renderer_quad.render(quad_Triangle.render_context);

    for(opengl::Quad2D& _quad : quads){
        renderer_quad.set_model_camera(_quad.transform_2d.matrix, camera.transform.matrix);
        renderer_quad.render(_quad.render_context);
    }
}



void Scene2D::handle_input(window::InputEvent event){
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

        bool zooming_in = event.mouse_scroll.delta > 0 ? true : false;

        if(zooming_in)
            camera.zoom_in();
        else 
            camera.zoom_out();


        camera.transform.set_matrix_camera();

    }

    
    if(event.type == window::EventType::MouseMove){
    
        if(mouse_pan){
            
            camera.pan(event.mouse_movement.delta);

        }
        
        // camera.set_cursor_pos(event.mouse_movement.pos_px, event.mouse_movement.pos_norm);
        cursor_context.set_cursor_pos(  event.mouse_movement.pos_px, 
                                        event.mouse_movement.pos_norm,
                                        camera.box
        );

    }


    if(event.type == window::EventType::WindowResize){

        window_size.x = event.window_resize.size_f.x;
        window_size.y = event.window_resize.size_f.y;

        camera.set_window_size_px(window_size);

    }

    if(event.key_stroke.key == window::Key::p && event.key_stroke.action == window::ButtonAction::Press){
        // quad.transform_2d.matrix.print();
        // camera.transform.matrix.print();

        // std::cout << "camera.transform.pos   = " << camera.transform.pos.x << " " << camera.transform.pos.y << std::endl;
        // std::cout << "camera.transform.scale = " << camera.transform.scale.x << " " << camera.transform.scale.y << std::endl;

        // std::cout << "camera.zoom_current = " << camera.zoom_current  << std::endl;

        camera.box.print();

        // std::cout << "camera.units_per_px = " << camera.units_per_px << std::endl;

        // std::cout << "camera.window_size_px = " << camera.window_size_px.x << " " << camera.window_size_px.y << std::endl;

        // std::cout << "camera.cursor_viewport_px    = " << camera.cursor_viewport_px.x << " " << camera.cursor_viewport_px.y << std::endl;
        // std::cout << "camera.cursor_viewport_norm  = " << camera.cursor_viewport_norm.x << " " << camera.cursor_viewport_norm.y << std::endl;
        // std::cout << "camera.cursor_viewport_scene = " << camera.cursor_viewport_scene.x << " " << camera.cursor_viewport_scene.y << std::endl;
        
        std::cout << "cursor_context.scene = " << cursor_context.scene.x << " " << cursor_context.scene.y << std::endl;
        
        // scene_2d.camera_2d.transform_2d.matrix.print();
        // std::cout << "mouse_pan = " << mouse_pan << std::endl;
        
    }
}


}
