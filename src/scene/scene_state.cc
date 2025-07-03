
#include "window/auxevent.hh"

#include "scene/scene3D.hh"
#include "scene/manager_3D.hh"

#include "scene/scene_state.hh"
#include "scene_state.hh"








bool SceneState::
try_change_selected(window::InputEvent & event)
{
    switch (selected.state)
    {
    case SS::Selected::KEY_1:

        if(selected.key_1 == SS::TRANSLATE_KEY)
        {
            if(event.is_mouse_scroll())
            {
                float increment = 1.0f;
                if(event.mouse_scroll.is_up())
                    selected.pos_delta += {0.0f, 0.0f,  increment};
                else
                    selected.pos_delta += {0.0f, 0.0f, -increment};
                return true;
            }
            if(event.is_mouse_movement())
            {
                f3 norm_delta = {   event.mouse_movement.delta.normalized, 
                                    0.0f                                    };
                selected.pos_delta += norm_delta;
                return true;
            }
        }
        if(selected.key_1 == SS::ROTATE_KEY)
        {
            if(event.is_mouse_movement())
            {
                selected.rot_delta_norm.x += event.mouse_movement.delta.normalized.x;
                selected.rot_delta_norm.y += event.mouse_movement.delta.normalized.y;
                return true;
            }
            if(event.is_mouse_scroll())
            {
                selected.rot_delta_norm.z += event.mouse_scroll.delta;
                return true;
            }
        }
        if(selected.key_1 == SS::SCALE_KEY)
        {
            if(event.is_mouse_scroll())
            {
                if(event.mouse_scroll.is_up())
                    selected.size_factor = 1.1f;
                else
                    selected.size_factor = 0.9f;
                return true;
            }
        }

        break;
    
    default:
        break;
    }
    return false;
}


bool SceneState::
try_move_camera(window::InputEvent & event)
{
    // Panning can only be done when not grabbing cursor
    if(camera.grabbing_cursor)
    {
        if(event.is_keystroke() && event.keystroke.is_press() && event.modifiers.is_none() && event.keystroke.is(Key::c))
        {
            camera.grabbing_cursor = false;
            return true;
        }
    }
    else
    {
        if(event.is_mouse_button() && event.mouse_button.is_middle_down())
        {
            camera.panning = true;
            return true;
        }
        if(event.is_mouse_button() && event.mouse_button.is_middle_up())
        {
            camera.panning = false;
            return true;
        }
        if(event.is_keystroke() && event.keystroke.is_press() && event.modifiers.is_none() && event.keystroke.is(Key::c) )
        {
            camera.grabbing_cursor = true;
            camera.panning = false;
            return true;
        }
    }

    


    // Mouse Navigation
    if(event.is_mouse_scroll())
    {
        camera.spherical_delta.x += event.mouse_scroll.delta;
        return true;
    }
    if(camera.grabbing_cursor || camera.panning)
    {
        if(event.is_mouse_movement())
        {
            camera.spherical_delta.y += event.mouse_movement.delta.sane.x;
            camera.spherical_delta.z += event.mouse_movement.delta.sane.y;
            return true;
        }
    }


    // Key Navigation
    if(event.is_keystroke())
    {
        if(event.keystroke.is_press())
        {
            switch (event.keystroke.key)
            {

            case CameraState::ForwardKey :
                camera.set_forward();
                return true;
                break;

            case CameraState::BackwardKey :
                camera.set_backward();
                return true;
                break;

            case CameraState::LeftKey :
                camera.set_left();
                return true;
                break;

            case CameraState::RightKey :
                camera.set_right();
                return true;
                break;

            case CameraState::UpKey :
                camera.set_up();
                return true;
                break;

            case CameraState::DownKey :
                camera.set_down();
                return true;
                break;
            
            default:
                break;
            }

        }

        if(event.keystroke.is_release())
        {
        
            switch (event.keystroke.key)
            {

            case CameraState::ForwardKey :
                camera.unset_forward();
                return true;
                break;

            case CameraState::BackwardKey :
                camera.unset_backward();
                return true;
                break;

            case CameraState::LeftKey :
                camera.unset_left();
                return true;
                break;

            case CameraState::RightKey :
                camera.unset_right();
                return true;
                break;

            case CameraState::UpKey :
                camera.unset_up();
                return true;
                break;

            case CameraState::DownKey :
                camera.unset_down();
                return true;
                break;
            
            default:
                break;
            }

        }
    }

    return false;
}





bool SceneState::
try_update_selected(window::InputEvent& event)
{
    if(event.is_mouse_button() && event.mouse_button.is_left_down())
    {
        selected.try_select(hovered.tag);
        return true;
    }
    if(event.is_keystroke() && event.keystroke.is_press() && event.keystroke.is_esc())
    {
        if(selected.try_peel())
            return true;
    }

    switch (selected.state)
    {
    case SS::Selected::SELECTED:
        {
        if(event.is_keystroke() && event.keystroke.is_press() && event.keystroke.is(SS::GRAB_KEY))
        {
            selected.set_key_1(SS::GRAB_KEY);
            return true;
        }
        if(event.is_keystroke() && event.keystroke.is_press() && event.keystroke.is(SS::ROTATE_KEY))
        {
            selected.set_key_1(SS::ROTATE_KEY);
            return true;
        }
        if(event.is_keystroke() && event.keystroke.is_press() && event.keystroke.is(SS::TRANSLATE_KEY))
        {
            selected.set_key_1(SS::TRANSLATE_KEY);
            return true;
        }
        if(event.is_keystroke() && event.keystroke.is_press() && event.keystroke.is(SS::SCALE_KEY))
        {
            selected.set_key_1(SS::SCALE_KEY);
            return true;
        }
        }
        break;

    case SS::Selected::KEY_1:
        if(event.is_keystroke() && event.keystroke.is_release() && event.keystroke.is(SS::GRAB_KEY))
        {
            selected.try_peel();
            return true;
        }
        if(event.is_keystroke() && event.keystroke.is_release() && event.keystroke.is(SS::ROTATE_KEY))
        {
            selected.try_peel();
            return true;
        }
        if(event.is_keystroke() && event.keystroke.is_release() && event.keystroke.is(SS::TRANSLATE_KEY))
        {
            selected.try_peel();
            return true;
        }
        if(event.is_keystroke() && event.keystroke.is_release() && event.keystroke.is(SS::SCALE_KEY))
        {
            selected.try_peel();
            return true;
        }
        break;

    case SS::Selected::KEY_2:
        break;

    default:
        break;
    }




    return false;
}


InputResponse SceneState::
handle_user_input(Manager3D& manager_3D, window::InputEvent & event, TagO sampled_tag)
{
    hovered.tag = sampled_tag;
    
    if(try_update_selected(event))
        goto consumed;
    
    if(selected.has_key())
        if(try_change_selected(event))
            goto consumed;
    
    if(try_move_camera(event))
        goto consumed;

        

    consumed:

    try_print(event);

    if(camera.grabbing_cursor)
        return InputResponse{InputResponse::MOUSE_GRAB};
    if(camera.panning)
        return InputResponse{InputResponse::MOUSE_PAN};

    return InputResponse();
}




void SceneState::
handle_window(Manager3D& manager_3D, window::WindowResizeEvent & window_event)
{
    manager_3D.window_scene->camera.perspective.set_fov(    window_event.size_i.x, 
                                                            window_event.size_i.y   );
}



void SceneState::
try_print(window::InputEvent& event)
{
    // PRINT INPUT
    if(event.is_keystroke() && event.keystroke.is_press())
    {
        switch (event.keystroke.key)
        {
        case Key::p :
            print_selected_state();
            // std::cout << "camera.view.rho    = " << scene.camera.view.rho << std::endl;
            // std::cout << "camera.theta  = " << camera.theta << std::endl;
            // std::cout << "camera.phi    = " << camera.phi << std::endl;

            // ground.transform.matrix.print();
            // camera.perspective_mat.print();
            // camera.view_mat.print();

            // event.cursor_pos.print();

            // opengl::error_check();
            break;
        
        default:
            break;
        }
    
    }
}



void SceneState::
print_selected_state()
{   
    std::cout << "Selected OID: " << selected.tag.oid << std::endl;
    std::string state_string;
    switch (selected.state)
    {
    case SS::Selected::EMPTY :
        state_string = "EMPTY";
        break;
    case SS::Selected::SELECTED :
        state_string = "SELECTED";
        break;
    case SS::Selected::KEY_1 :
        state_string = "KEY_1";
        break;
    case SS::Selected::KEY_2 :
        state_string = "KEY_2";
        break;
    
    default:
        break;
    }
    std::cout << "State: " << state_string << std::endl;
    
    std::cout << "key 1: " << (char) keys.key_1 << std::endl;
    std::cout << "key 2: " << (char) keys.key_2 << std::endl << std::endl;
}
