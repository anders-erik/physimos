#include "glad/glad.h"
#include "scene3D.hh"
#include "camera.hh"

#include "opengl/error.hh"

#include "renderer_axes.hh"


#include "model.hh"
#include "math/vecmat.hh"


#include <iostream>
#include <cmath>




void Scene3D::push_object(Object new_object)
{
    objects.push_back(new_object);
}

std::vector<Object>& Scene3D::get_objects_mut()
{
    return objects;
}



void Scene3D::push_texture_model(model::ModelT & new_model)
{
    texture_models.push_back(new_model);
}

void Scene3D::push_color_model(model::ModelColor & new_model)
{
    color_models.push_back(new_model);
}


void Scene3D::update()
{
}




InputResponse Scene3D::handle_input(window::InputEvent input_event){


    switch (input_event.type){

    case window::EventType::MouseButton:
        
        if(input_event.mouse_button.is_middle_down())
        {
            camera_grabbed = true;
            // std::cout << "GRAB CAMERA" << std::endl;
        }

        if(input_event.mouse_button.is_middle_up())
        {
            camera_grabbed = false;
            // std::cout << "RELEASE CAMERA" << std::endl;
        }

        if(input_event.mouse_button.is_left_down())
        {
            selected_object = hovered_object;
        }
        break;


    case window::EventType::MouseMove:

        if(camera_grabbed){
            // std::cout << "Orbiting" << std::endl;
            
            camera.view.theta_change(input_event.mouse_movement.delta.sane.x);
            camera.view.phi_change(input_event.mouse_movement.delta.sane.y);
        }


        break;
    
    case window::EventType::MouseScroll:

        camera.view.rho_change(input_event.mouse_scroll.delta);

        break;

    case window::EventType::Keystroke:

        if(input_event.keystroke.action == window::ButtonAction::Hold){
            // std::cout << "KEY HOLD" << std::endl;
        }
        else if(input_event.keystroke.action == window::ButtonAction::Press){
            // std::cout << "KEY PRESS" << std::endl;
        
            switch (input_event.keystroke.key){

            case window::Key::p :
                std::cout << "camera.view.rho    = " << camera.view.rho << std::endl;
                // std::cout << "camera.theta  = " << camera.theta << std::endl;
                // std::cout << "camera.phi    = " << camera.phi << std::endl;

                // ground.transform.matrix.print();
                // camera.perspective_mat.print();
                // camera.view_mat.print();

                opengl::error_check();
                break;
            
            default:
                break;
            }
        
        }
        else if(input_event.keystroke.action == window::ButtonAction::Release){
            // std::cout << "KEY RELEASE" << std::endl;
        }


        break;

    }

    // Tell caller that we are still grabbing
    if(camera_grabbed)
    {
        return InputResponse {InputResponse::MOUSE_GRAB};
    }

    return InputResponse {};
}

void Scene3D::handle_window_resize(window::WindowResizeEvent resize_event)
{
    camera.perspective.set_fov(resize_event.size_i.x, resize_event.size_i.y);
}



