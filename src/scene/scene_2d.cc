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

void Camera2D::set_aspect_ratio(float ar){
    aspect_ratio = ar;
    // std::cout << "aspect_ratio = " << aspect_ratio << std::endl;
    
    transform.scale.y = transform.scale.x * aspect_ratio;
}

void Camera2D::set_zoom_multiplier(float multiplier){
    zoom_multiplier = multiplier;
}
void Camera2D::zoom_set(float zoom){
    zoom_current = zoom;

    transform.scale.x = 1 / zoom_current;
    transform.scale.y = transform.scale.x * aspect_ratio;
}

void Camera2D::zoom_in(){
    zoom_current *= zoom_multiplier;

    transform.scale.x = zoom_current;
    transform.scale.y = transform.scale.x * aspect_ratio;
}
void Camera2D::zoom_out(){
    zoom_current *= 1.0f / zoom_multiplier;

    transform.scale.x = zoom_current;
    transform.scale.y = transform.scale.x * aspect_ratio;
}


Scene2D::Scene2D()
    : texture_framebuffer_F { phont::get_texture_F() }
{

    renderer_quad.create_context(quad);
	

	quad.transform_2d.set_pos(0.0f, 0.0f);
	quad.transform_2d.set_scale(2.0f, 3.0f);

	quad.render_context.texture = texture_framebuffer_F.texture_id;


	camera.transform.set_pos	 (0.0f, 0.0f);
	// camera.transform.set_scale(1.0f, 1.0f);
    camera.zoom_set(1.0f);
    camera.set_zoom_multiplier(1.2f);

}


void Scene2D::set_window_size(i2 size){
    window_size = size;

    float aspect_ratio = (float) size.x / (float) size.y;

    camera.set_aspect_ratio(aspect_ratio);
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
            float pan_scale = 0.003f;
            camera.transform.move_x(- event.mouse_movement.delta.x * pan_scale);
            camera.transform.move_y(- event.mouse_movement.delta.y * pan_scale);
            camera.transform.set_matrix_camera();
        }
    }


    if(event.type == window::EventType::WindowResize){

        window_size.x = event.window_resize.size.x;
        window_size.y = event.window_resize.size.y;

        float aspect_ratio = (float) event.window_resize.size.x / (float) event.window_resize.size.y;

        camera.set_aspect_ratio(aspect_ratio);

    }

    if(event.key_stroke.key == window::Key::p && event.key_stroke.action == window::ButtonAction::Press){
        // quad.transform_2d.matrix.print();
        // camera.transform.matrix.print();

        // std::cout << " " << camera.transform.pos.x << " " << camera.transform.pos.y << std::endl;
        // std::cout << " " << camera.transform.scale.x << " " << camera.transform.scale.y << std::endl;

        std::cout << "camera.zoom_current = " << camera.zoom_current  << std::endl;
        
        // scene_2d.camera_2d.transform_2d.matrix.print();
        // std::cout << "mouse_pan = " << mouse_pan << std::endl;
        
    }
}


}
