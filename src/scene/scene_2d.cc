#include "glad/glad.h"
#include "scene/scene_2d.hh"

#include "camera.hh"

#include "opengl/error.hh"

#include "renderer_axes.hh"


#include "model.hh"
#include "math/vecmat.hh"

#include "phont/phont.hh"

// #include "rubik.hh"

#include <iostream>
#include <cmath>

namespace scene {

void Box2D::print(){
    std::cout << "Box2D: " << std::endl;
    std::cout << "box.pos.x  = " << pos.x << std::endl;
    std::cout << "box.pos.y  = " << pos.y << std::endl;
    std::cout << "box.size.x = " << size.x << std::endl;
    std::cout << "box.size.y = " << size.y << std::endl;
    
}

void Camera2D::reload_dims(){

    box.size.x = width_initial / zoom_current;
    box.size.y = box.size.x / aspect_ratio;

    units_per_px = box.size.x / window_size_px.x;

    transform.scale.x = zoom_current;
    transform.scale.y = transform.scale.x * aspect_ratio;
}

void Camera2D::set_window_size_px(f2 size){
    window_size_px.x = size.x;
    window_size_px.y = size.y;

    aspect_ratio = window_size_px.x / window_size_px.y;

    reload_dims();
}

void Camera2D::set_zoom_multiplier(float multiplier){
    zoom_multiplier = multiplier;
}

void Camera2D::zoom_set(float zoom){
    zoom_current = zoom;
    reload_dims();
}

void Camera2D::zoom_in(){
    zoom_current *= zoom_multiplier;
    reload_dims();
}

void Camera2D::zoom_out(){
    zoom_current *= 1.0f / zoom_multiplier;
    reload_dims();
}

void Camera2D::pan(f2 delta_px){

    // std::cout << "box.pos dx = " << units_per_px * delta_px.x << std::endl;

    float dx_scene_units = units_per_px * delta_px.x;
    float dy_scene_units = units_per_px * delta_px.y;

    box.pos.x -= dx_scene_units;
    box.pos.y -= dy_scene_units;

    // float pan_scale = 0.003f;
    // float pan_scale_x = 2.0f / window_size_px.x;
    // float pan_scale_y = 2.0f / window_size_px.y;
    // transform.move_x(- delta_px.x * pan_scale_x);
    // transform.move_y(- delta_px.y * pan_scale_y);
    // transform.set_matrix_camera();

    transform.pos.x = box.pos.x;
    transform.pos.y = box.pos.y;
    transform.set_matrix_camera();
}

void Camera2D::set_cursor_pos(f2 pos_px, f2 pos_norm){
    cursor_viewport_px = pos_px;
    cursor_viewport_norm = pos_norm;

    cursor_viewport_scene.x = box.pos.x + (cursor_viewport_norm.x - 0.5) * box.size.x ;
    cursor_viewport_scene.y = box.pos.y + (cursor_viewport_norm.y - 0.5) * box.size.y ;
}


Scene2D::Scene2D(f2 _window_size)
    : texture_framebuffer_F { phont::get_texture_F() }
{
    set_window_size(_window_size);

    renderer_quad.create_context(quad);
	

	quad.transform_2d.set_pos(0.0f, 0.0f);
	quad.transform_2d.set_scale(2.0f, 3.0f);

	quad.render_context.texture = texture_framebuffer_F.texture_id;


	camera.box.pos = { 0.0f, 0.0f };
    camera.pan({ 0.0f, 0.0f});
	// camera.transform.set_scale(1.0f, 1.0f);

    // camera.zoom_set(0.1f);
    camera.set_zoom_multiplier(1.2f);
    camera.set_window_size_px(window_size);


    // camera.box.pos = {-8.0f, -8.0f};

}


void Scene2D::set_window_size(f2 size){
    window_size = size;

    camera.set_window_size_px(size);
}

void Scene2D::update(){

    quad.transform_2d.set_matrix_model();
    camera.transform.set_matrix_camera();

    renderer_quad.set_model_camera(quad.transform_2d.matrix, camera.transform.matrix);
}


void Scene2D::render(){
    // RENDER
    renderer_quad.set_model_camera(quad.transform_2d.matrix, camera.transform.matrix);
    renderer_quad.render(quad.render_context);
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
        
        camera.set_cursor_pos(event.mouse_movement.pos_px, event.mouse_movement.pos_norm);
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
        std::cout << "camera.cursor_viewport_scene = " << camera.cursor_viewport_scene.x << " " << camera.cursor_viewport_scene.y << std::endl;
        
        
        // scene_2d.camera_2d.transform_2d.matrix.print();
        // std::cout << "mouse_pan = " << mouse_pan << std::endl;
        
    }
}


}
