
#include "scene/scene3D.hh"

#include "scene/scene_state.hh"


InputResponse SceneState::
old_scene_handler(Scene3D& scene, window::InputEvent & event)
{
    switch (event.type){

    case window::EventType::MouseButton:

        if(event.mouse_button.is_middle_down())
        {
            camera_grabbed = true;
            // std::cout << "GRAB CAMERA" << std::endl;
        }

        if(event.mouse_button.is_middle_up())
        {
            camera_grabbed = false;
            // std::cout << "RELEASE CAMERA" << std::endl;
        }

        if(event.mouse_button.is_left_down())
        {
            selected_tag = hovered_tag;
        }
        break;


    case window::EventType::MouseMove:

        if(camera_grabbed){
            // std::cout << "Orbiting" << std::endl;
            
            scene.camera.view.theta_change(event.mouse_movement.delta.sane.x);
            scene.camera.view.phi_change(event.mouse_movement.delta.sane.y);
        }


        break;
    
    case window::EventType::MouseScroll:

        scene.camera.view.rho_change(event.mouse_scroll.delta);

        break;

    case window::EventType::Keystroke:

        if(event.keystroke.action == window::ButtonAction::Hold){
            // std::cout << "KEY HOLD" << std::endl;
        }
        else if(event.keystroke.action == window::ButtonAction::Press){
            // std::cout << "KEY PRESS" << std::endl;

            switch (event.keystroke.key){

            case window::Key::p :
                std::cout << "camera.view.rho    = " << scene.camera.view.rho << std::endl;
                // std::cout << "camera.theta  = " << camera.theta << std::endl;
                // std::cout << "camera.phi    = " << camera.phi << std::endl;

                // ground.transform.matrix.print();
                // camera.perspective_mat.print();
                // camera.view_mat.print();

                // opengl::error_check();
                break;
            
            default:
                break;
            }
        
        }
        else if(event.keystroke.action == window::ButtonAction::Release){
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

InputResponse SceneState::
handle(Scene3D& scene, window::InputEvent & event)
{
    // scene.handle_input(event);
    old_scene_handler(scene, event);
    return InputResponse();
}



void SceneState::
handle_window(Scene3D& scene, window::WindowResizeEvent & window_event)
{
    scene.camera.perspective.set_fov(   window_event.size_i.x, 
                                        window_event.size_i.y );
}
