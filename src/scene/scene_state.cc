
#include "scene/scene3D.hh"
#include "scene/manager_3D.hh"

#include "scene/scene_state.hh"
#include "scene_state.hh"


InputResponse SceneState::
send_to_current_state(Scene3D& scene, window::InputEvent & event)
{
    switch (event.type)
    {

    case window::EventType::MouseMove:

        if(cursor_grab)
        {
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

            case Key::p :
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
    bool quad_release_esc = event.is_keystroke() && event.keystroke.is(Key::Esc);

    if(quad_release_esc)
    {
        active_tags.release_quad();
		return true;
    }

    return false;
}



void SceneState::
peel_one_layer()
{
    
}


void SceneState::
clear_all_layers()
{
    active_tags.release_quad();
    active_tags.unselect();
    keys.clear_all();
    cursor_grab = false;
}

bool SceneState::
try_clear_state(window::InputEvent& event)
{
    // A left click automatically clears all scene states
    if(event.is_mouse_button() && event.mouse_button.is_left_down())
    {
        clear_all_layers();
        return true;
    }

    return false;
}

bool SceneState::
try_peel_state(window::InputEvent& event)
{
    // Escape always peels once layer
    if(event.is_keystroke() && event.keystroke.is_esc())
    {
        if(cursor_grab)
        {
            cursor_grab = false;
        }
        else if(keys.key_2_active())
        {
            keys.clear_2();
        }
        else if(keys.key_1_active())
        {
            keys.clear_1();
        }
        else if(active_tags.is_quad_capture())
        {
            active_tags.release_quad();
        }
        else if(active_tags.is_selected_not_capture())
        {
            active_tags.unselect();
        }

        return true;
    }

    // State specific peeling
    if(cursor_grab)
    {
        if(event.is_mouse_button() && event.mouse_button.is_middle_up())
            cursor_grab = false;
        else if(event.is_keystroke() && event.keystroke.is_esc())
            cursor_grab = false;
        else
            return false;
    }

    return false;
}


bool SceneState::
try_build_state(window::InputEvent& event)
{
    if(event.is_mouse_button() && event.mouse_button.is_left_down())
    {
        active_tags.select_current_hover();
    }

    if(event.is_keystroke() && event.keystroke.is(Key::alt))
    {
        cursor_grab = true;
        return true;
    }
    if(event.is_mouse_button() && event.mouse_button.is_middle_down())
    {
        cursor_grab = true;
        return true;
    }

    return false;
}



InputResponse SceneState::handle_user_input(Manager3D & manager_3D, window::InputEvent & event)
{
    try_clear_state(event);
    
    try_peel_state(event);

    try_build_state(event);


    send_to_current_state( *manager_3D.window_scene, 
                            event                   );

    if(cursor_grab)
        return InputResponse{InputResponse::MOUSE_GRAB};
    
    return InputResponse();
}



void SceneState::
handle_window(Manager3D& manager_3D, window::WindowResizeEvent & window_event)
{
    manager_3D.window_scene->camera.perspective.set_fov(   window_event.size_i.x, 
                                        window_event.size_i.y );
}


void SceneState::
print_key_state()
{
    std::cout << "key 1: " << (char) keys.key_1 << std::endl;
    std::cout << "key 2: " << (char) keys.key_2 << std::endl << std::endl;
}
