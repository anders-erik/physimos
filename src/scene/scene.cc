#include "glad/glad.h"
#include "scene.hh"
#include "camera.hh"

#include "opengl/error.hh"

#include "renderer_axes.hh"


#include "model.hh"
#include "math/vecmat.hh"

// #include "rubik.hh"

#include <iostream>
#include <cmath>




model::TubeContext tube_context = {
    1.0f,   //  float radius;
    6,      //  int frame_point_count;
    10,     //  int frame_count;
    1.0,    //  float frame_gap;
    0,      //  int vertex_count_target;
};

SceneModelEditor::SceneModelEditor(){

    opengl::textures_init();

    renderer_model_color.init();
    renderer_axes.init();
    renderer_model_texture.init();
    renderer_vector.init();
    

    // GROUND
    model_generate_ground(ground.mesh);
    model_center(ground.mesh);
    renderer_model_texture.create_model_rendering_context(ground.mesh, ground_render_context, opengl::Textures::Grass);
    
    // TUBE
    model_generate_tube(tube.mesh, tube_context);
    model_center(tube.mesh);
    model_scale(tube.mesh, 0.5f);
    // model_rotate(tube.mesh, 0.5f , {1.0f, 0.0f, 0.0f});
    model_translate(tube.mesh, {-3.0f, 0.0f, 2.0f});
    renderer_model_texture.create_model_rendering_context(tube.mesh, tube_render_context, opengl::Textures::Colors );


    // COLOR MODEL
    model_add_cube_mesh(model.mesh);
    renderer_model_color.create_render_context(model_render_context, model);
    model.transform.pos.x = 5.0f;

}


void SceneModelEditor::update(){

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
    // camera.set_triplet(camera.rho, camera.theta, camera.phi);
    // camera.triplet_reload();

    camera.set_matrices();


    renderer_model_color.set_camera_uniforms(camera.view_mat, camera.perspective_mat);
    renderer_model_texture.set_camera_view_projection(camera.perspective_mat, camera.view_mat);
    renderer_vector.set_project_view_matrix(camera.perspective_mat, camera.view_mat);


    m4f4 indentity;
    renderer_axes.set_uniforms(indentity, camera.view_mat, camera.perspective_mat);
    // renderer_axes.set_uniforms(_cube.c0.model.transform.matrix, camera.view_mat, camera.perspective_mat);
    
    // model.set_transform_matrix();
    model.transform.set_matrix_model();
    ground.transform.set_matrix_model();
    tube.transform.set_matrix_model();

}

void SceneModelEditor::render(){


    m4f4 identity;
    renderer_axes.set_uniforms(identity, camera.view_mat, camera.perspective_mat);
    renderer_axes.render();


    renderer_vector.render({0.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f});

    // renderer_vector.render({0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 1.0f});

    // TUBE NORMAL VECTORS
    for(model::VertexT vertex : tube.mesh.vertices){
        renderer_vector.render(vertex.normal, vertex.pos);
    }



    
    // glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
    // glPointSize(5);
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // glLineWidth(2);
    // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    renderer_model_color.render(model_render_context, model.transform.matrix, model.mesh.faces.size()*3);


    // renderer_model_texture.render(ground.mesh, ground.transform.matrix);
    renderer_model_texture.render_model_rendering_context(ground.mesh, ground_render_context, ground.transform.matrix);
    

    
    renderer_model_texture.render_model_rendering_context(tube.mesh, tube_render_context, tube.transform.matrix);
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // glLineWidth(2);
    // renderer_model_texture.render_model_rendering_context(tube.mesh, tube_render_context, tube.transform.matrix);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
};



void SceneModelEditor::handle_input(window::InputEvent input_event){


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
                // std::cout << "camera.rho    = " << camera.rho << std::endl;
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

}

void SceneModelEditor::handle_window_resize(window::WindowResizeEvent resize_event)
{
    camera.set_fov(resize_event.size_i.x, resize_event.size_i.y);
}



