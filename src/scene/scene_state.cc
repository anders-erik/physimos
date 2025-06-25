
#include "scene/scene3D.hh"
#include "scene/manager_3D.hh"

#include "scene/scene_state.hh"
#include "scene_state.hh"







bool SceneState::
update_state(window::InputEvent& event, TagO sampled_tag)
{
    active_tags.hover_set(sampled_tag);


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
        else
        {
            return false;
        }

        return true;
    }

    // State specific peeling
    if(active_tags.has_selected())
    {
        
    }


    if(event.is_mouse_button() && event.mouse_button.is_left_down())
        active_tags.select(active_tags.tag_hover);


    return false;
}


void SceneState::
camera_movement(window::InputEvent & event)
{
    // Panning can only be done when not grabbing cursor
    if(camera.grabbing_cursor)
    {
        if(event.is_keystroke() && event.keystroke.is_press() && event.modifiers.is_none() && event.keystroke.is(Key::c) )
            camera.grabbing_cursor = false;
    }
    else
    {
        if(event.is_mouse_button() && event.mouse_button.is_middle_down())
            camera.panning = true;
        if(event.is_mouse_button() && event.mouse_button.is_middle_up())
            camera.panning = false;
        
        if(event.is_keystroke() && event.keystroke.is_press() && event.modifiers.is_none() && event.keystroke.is(Key::c) )
        {
            camera.grabbing_cursor = true;
            camera.panning = false;
        }
    }

    


    // Mouse Navigation
    if(event.is_mouse_scroll())
        camera.spherical_delta.x += event.mouse_scroll.delta;

    if(camera.grabbing_cursor || camera.panning)
    {
        if(event.is_mouse_movement())
        {
            camera.spherical_delta.y += event.mouse_movement.delta.sane.x;
            camera.spherical_delta.z += event.mouse_movement.delta.sane.y;
        }
    }

    // Key Navigation
    if(event.is_keystroke())
    {
        switch (event.keystroke.key)
        {

        case CameraState::ForwardKey :
            if(event.keystroke.is_press())
                camera.set_forward();
            if(event.keystroke.is_release())
                camera.unset_forward();
            break;

        case CameraState::BackwardKey :
            if(event.keystroke.is_press())
                camera.set_backward();
            if(event.keystroke.is_release())
                camera.unset_backward();
            break;

        case CameraState::LeftKey :
            if(event.keystroke.is_press())
                camera.set_left();
            if(event.keystroke.is_release())
                camera.unset_left();
            break;

        case CameraState::RightKey :
            if(event.keystroke.is_press())
                camera.set_right();
            if(event.keystroke.is_release())
                camera.unset_right();
            break;

        case CameraState::UpKey :
            if(event.keystroke.is_press())
                camera.set_up();
            if(event.keystroke.is_release())
                camera.unset_up();
            break;

        case CameraState::DownKey :
            if(event.keystroke.is_press())
                camera.set_down();
            if(event.keystroke.is_release())
                camera.unset_down();
            break;
        
        default:
            break;
        }
    }

}





InputResponse SceneState::handle_user_input(Manager3D& manager_3D, window::InputEvent & event, TagO sampled_tag)
{
    update_state(event, sampled_tag);

    camera_movement(event);


    // PRINT INPUT
    if(event.is_keystroke() && event.keystroke.is_press())
    {
        switch (event.keystroke.key)
        {
        case Key::p :
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

    if(camera.grabbing_cursor)
        return InputResponse{InputResponse::MOUSE_GRAB};
    if(camera.panning)
        return InputResponse{InputResponse::MOUSE_PAN};

    else
        return InputResponse();
}



void SceneState::
handle_window(Manager3D& manager_3D, window::WindowResizeEvent & window_event)
{
    manager_3D.window_scene->camera.perspective.set_fov(    window_event.size_i.x, 
                                                            window_event.size_i.y   );
}


void SceneState::
print_key_state()
{
    std::cout << "key 1: " << (char) keys.key_1 << std::endl;
    std::cout << "key 2: " << (char) keys.key_2 << std::endl << std::endl;
}
