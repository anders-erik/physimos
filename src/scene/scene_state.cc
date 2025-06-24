
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
try_peel_state(window::InputEvent& event)
{
    // Escape always peels once layer
    if(event.is_keystroke() && event.keystroke.is_esc())
    {
        if(keys.key_2_active())
        {
            keys.clear_2();
        }
        else if(keys.key_1_active())
        {
            keys.clear_1();
        }
        else if(active_tags.has_selected())
        {
            active_tags.unselect();
        }

        return true;
    }

    // State specific peeling
    if(active_tags.has_selected())
    {
        
    }

    return false;
}


bool SceneState::
try_build_state(window::InputEvent& event)
{
    

    return false;
}



InputResponse SceneState::handle_user_input(Manager3D& manager_3D, window::InputEvent & event, TagO sampled_tag)
{
    active_tags.hover_set(sampled_tag);

    try_peel_state(event);

    try_build_state(event);


    if(event.is_mouse_button() && event.mouse_button.is_left_down())
        active_tags.select(sampled_tag);

    if(event.is_keystroke() && event.keystroke.is(Key::alt) && event.keystroke.is_press())
        cursor_grab = !cursor_grab;

    if(event.is_mouse_button() && event.mouse_button.is_middle_down())
        cursor_grab = true;
    if(event.is_mouse_button() && event.mouse_button.is_middle_up())
        cursor_grab = false;


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
