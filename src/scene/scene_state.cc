
#include "scene/scene3D.hh"
#include "scene/manager_3D.hh"

#include "scene/scene_state.hh"
#include "scene_state.hh"


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
            active_tags.select_current_hover();
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


bool SceneState::
try_new_quad_grab(window::InputEvent& event)
{
    if(event.is_mouse_button() && event.mouse_button.is_left_down() && event.modifiers.is_ctrl())
    {
        if(active_tags.is_hovering_a_quad())
        {
            active_tags.capture_current_hover();
            return true;
        }
    }

	return false;
}


bool SceneState::
try_release_quad(window::InputEvent& event)
{
    bool quad_release_click = event.is_mouse_button() && event.mouse_button.is_left_down();

    // If NOT hovering grabbing/capturing quad and left click is failed grab
    if(!active_tags.is_hovering_captured() && quad_release_click)
    {
        active_tags.release_quad();
		return true;
    }

    return false;
}


bool SceneState::
try_release_quad_esc(window::InputEvent& event)
{
    bool quad_release_esc = event.is_keystroke() && event.keystroke.is(window::Key::Esc);

    if(quad_release_esc)
    {
        active_tags.release_quad();
		return true;
    }

    return false;
}


InputResponse SceneState::
handle_mouse(Manager3D& manager_3D, window::InputEvent & event)
{

    old_scene_handler( *manager_3D.window_scene, 
                        event                   );

    if(camera_grabbed)
        return InputResponse{InputResponse::MOUSE_GRAB};

    return InputResponse();
}


InputResponse SceneState::
handle_key(Manager3D & manager_3D, window::InputEvent & event)
{
    bool esc = event.is_keystroke() && event.keystroke.is_press() && event.keystroke.is(window::Key::Esc);

    if(esc)
    {
        if(active_tags.is_quad_capture())
            active_tags.release_quad();
        else if(active_tags.is_selected_not_capture())
            active_tags.unselect();
    }


    return InputResponse();
}

void SceneState::
handle_window(Manager3D& manager_3D, window::WindowResizeEvent & window_event)
{
    manager_3D.window_scene->camera.perspective.set_fov(   window_event.size_i.x, 
                                        window_event.size_i.y );
}
