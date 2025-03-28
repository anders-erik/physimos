#include "glad/glad.h"
#include "scene.hh"
#include "camera.hh"

#include "renderer_axes.hh"


#include "model.hh"
#include "math/math.hh"

// #include "rubik.hh"

#include <iostream>
#include <cmath>

namespace xpeditor {



void Scene::set_viewport_dims(int _width, int _height){
    // std::cout << _width << _height << std::endl;
    camera.width = (float) _width;
    camera.height = (float) _height;
}

Scene::Scene(){

    renderer_model.init();
    renderer_axes.init();

    // camera.transform.rot.z = 0.5;
    camera.transform.rot.x =  1.3;
    camera.transform.rot.y =  0.0;
    camera.transform.rot.z = -0.75;

    camera.transform.pos.x = -4.0;
    camera.transform.pos.y = -4.0;
    camera.transform.pos.z = 2.0f;

    model_add_cube_mesh(model.mesh);
    renderer_model.create_render_context(model);

    // renderer_model.create_render_context(_cube.c_xp.model);
    // renderer_model.create_render_context(_cube.c1.model);
    

    // return true;
}

void Scene::handle_input(window::InputEvent input_event){


    switch (input_event.type){

    case window::EventType::MouseButton:
        
        if(input_event.mouse_button.button == window::MouseButton::Middle){

            if(input_event.mouse_button.action == window::ButtonAction::Press)
                camera_grabbed = true;
                // std::cout << "GRAB CAMERA" << std::endl;

            if(input_event.mouse_button.action == window::ButtonAction::Release)
                camera_grabbed = false;
                // std::cout << "RELEASE CAMERA" << std::endl;


        }

        break;


    case window::EventType::MouseMove:

        if(camera_grabbed){
            camera.theta_change(input_event.mouse_movement.delta.x);
            camera.phi_change(input_event.mouse_movement.delta.y);
        }
        

        break;
    
    case window::EventType::MouseScroll:

        camera.rho_change(input_event.mouse_scroll.delta);

        break;

    case window::EventType::Keystroke:

        if(input_event.key_stroke.action == window::ButtonAction::Hold){
            // std::cout << "KEY HOLD" << std::endl;
        }
        else if(input_event.key_stroke.action == window::ButtonAction::Press){
            // std::cout << "KEY PRESS" << std::endl;
        
            switch (input_event.key_stroke.key){

            case window::Key::p :
                std::cout << "camera.rho    = " << camera.rho << std::endl;
                std::cout << "camera.theta  = " << camera.theta << std::endl;
                std::cout << "camera.phi    = " << camera.phi << std::endl;
                break;
            
            default:
                break;
            }
        
        }
        else if(input_event.key_stroke.action == window::ButtonAction::Release){
            // std::cout << "KEY RELEASE" << std::endl;
        }


        

        break;

    case window::EventType::WindowResize:

        std::cout << "WIN RESIZE" << std::endl;

        break;

    }

}

void Scene::update(){

    // if(_cube.animator.is_animating)
        // _cube.update_animator();
    // else
        // cube
    // _cube.update_cubies();
    
    // _cube.c_xp.model.set_transform_matrix();
    // _cube.c_xp.tr

    // _cube.c0.model.set_transform_matrix();
    // _cube.c1.model.set_transform_matrix();

    // camera
    camera.set_triplet(camera.rho, camera.theta, camera.phi);
    camera.set_matrices();
    renderer_model.set_camera_uniforms(camera.view_mat, camera.perspective_mat);

    m4f4 indentity;
    renderer_axes.set_uniforms(indentity, camera.view_mat, camera.perspective_mat);
    // renderer_axes.set_uniforms(_cube.c0.model.transform.matrix, camera.view_mat, camera.perspective_mat);


}

void Scene::render(){


    m4f4 identity;
    renderer_axes.set_uniforms(identity, camera.view_mat, camera.perspective_mat);
    renderer_axes.render();
    // renderer_axes.set_uniforms(_cube.cubies[0].model.transform.matrix, camera.view_mat, camera.perspective_mat);
    // renderer_axes.render();

    // for(Cubie& c : _cube.cubies){
    //     renderer_model.render(c.model);
    // }
    // renderer_model.render(_cube.c_xp.model);
}




}
