#include "glad/glad.h"
#include "scene3D.hh"
#include "camera.hh"

#include "opengl/error.hh"

#include "renderer_axes.hh"


#include "model.hh"
#include "math/vecmat.hh"


#include <iostream>
#include <cmath>




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
        
        if(input_event.mouse_button.button == window::MouseButton::Middle){

            if(input_event.mouse_button.action == window::ButtonAction::Press)
            {
                camera_grabbed = true;
                std::cout << "GRAB CAMERA" << std::endl;
            }

            if(input_event.mouse_button.action == window::ButtonAction::Release)
            {
                camera_grabbed = false;
                std::cout << "RELEASE CAMERA" << std::endl;
            }

        }

        break;


    case window::EventType::MouseMove:

        if(camera_grabbed){
            // std::cout << "Orbiting" << std::endl;
            
            camera.theta_change(input_event.mouse_movement.delta.sane.x);
            camera.phi_change(input_event.mouse_movement.delta.sane.y);
        }


        break;
    
    case window::EventType::MouseScroll:

        camera.rho_change(input_event.mouse_scroll.delta);

        break;

    case window::EventType::Keystroke:

        if(input_event.keystroke.action == window::ButtonAction::Hold){
            // std::cout << "KEY HOLD" << std::endl;
        }
        else if(input_event.keystroke.action == window::ButtonAction::Press){
            // std::cout << "KEY PRESS" << std::endl;
        
            switch (input_event.keystroke.key){

            case window::Key::p :
                std::cout << "camera.rho    = " << camera.rho << std::endl;
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
    camera.set_fov(resize_event.size.x, resize_event.size.y);
}



