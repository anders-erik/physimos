#include <iostream>
#include <list>

#include "lib/log.hh"

#include "math/quarternion.hh"

#include "rend/rend_manager.hh"

#include "scene/scene3D.hh"

#include "scene/manager_3D.hh"
#include "manager_3D.hh"




/** 
    Returns a unique, not equal to 0 or 1, unsigned integer.
    An id of 0 is globally understood as having no object asssociated with it; ids of 1 is always the fixed root scene.
 */
SID Manager3D::
new_sid_3D()
{
    ++sid_3D_counter;

    if (sid_3D_counter == 0)
        throw std::runtime_error("SID 3d counter was equal to 0.");
    else if (sid_3D_counter == 1)
        throw std::runtime_error("SID 3d counter was equal to 1.");
    
    return sid_3D_counter | SID_3D;
}



Scene3D& Manager3D::
init(f2 window_scene_f)
{
    if(init_flag)
        throw std::runtime_error("Can't init the scene manager twice");
    init_flag = true;

    root_scene.sid = 1 | SID_3D;

    window_scene = &root_scene;

    renderer_3D.init(window_scene_f);

    return root_scene;
}



void Manager3D::
update()
{
    // CAMERA
    auto& camera = window_scene->camera;

    if(state.camera.is_forward())
        window_scene->camera.forward(0.15);
    if(state.camera.is_backward())
        window_scene->camera.backward(0.15);
    if(state.camera.is_left())
        window_scene->camera.left(0.03);
    if(state.camera.is_right())
        window_scene->camera.right(0.03);
    if(state.camera.is_up())
        window_scene->camera.up(0.02);
    if(state.camera.is_down())
        window_scene->camera.down(0.02);

    window_scene->camera.view.rho_change(   -state.camera.spherical_delta.x  / 1.0f    );
    window_scene->camera.view.theta_change( -state.camera.spherical_delta.y  / 200.0f    );
    window_scene->camera.view.phi_change(    state.camera.spherical_delta.z  / 400.0f      );
    state.camera.spherical_delta.set_zero();


    // SELECTED
    Object* selected_o = manager_o.get_object(state.selected.tag);
    if(selected_o != nullptr)
    {
        // CAMERA DIRS
        f3 cam_r    = window_scene->camera.get_right();
        f3 cam_u    = window_scene->camera.get_up();
        f3 cam_f    = window_scene->camera.get_forward();
    

        // 1. TRANSLATE
        if(!state.selected.pos_delta.is_zero())
        {
            // SCALE MOVEMENT
            f3 cam_pos          = window_scene->camera.get_pos();
            f3 cam_to_obj_delta = cam_pos - selected_o->pos;
            float tan_half_fov = tanf(camera.perspective.fov / 2.0f);
            float view_width_at_obj_dist = 2.0f * tan_half_fov * cam_to_obj_delta.norm();

            selected_o->pos += cam_r * state.selected.pos_delta.x * view_width_at_obj_dist * camera.perspective.AR();
            selected_o->pos += cam_u * state.selected.pos_delta.y * view_width_at_obj_dist;
            selected_o->pos += cam_f * state.selected.pos_delta.z;

            state.selected.pos_delta.set_zero();
        }


        // 2. ROTATE
        if(!state.selected.rot_delta_norm.is_zero())
        {
            float move_factor = 10.0f;
            float scroll_factor = 0.20f;

            f3 angle_delta = state.selected.rot_delta_norm;
            angle_delta.x *= move_factor;
            angle_delta.y *= move_factor;
            angle_delta.z *= scroll_factor;

            selected_o->rot.rotate(cam_u,  angle_delta.x);
            selected_o->rot.rotate(cam_r, -angle_delta.y);
            selected_o->rot.rotate(cam_f, -angle_delta.z);
            
            state.selected.rot_delta_norm.set_zero();
        }

        // 3. SCALE
        if(state.selected.size_factor != 0.0f)
        {
            selected_o->scale *= state.selected.size_factor;
            state.selected.size_factor = 0.0f;
        }
    }
}


void Manager3D::
render_window_scene (   
                        i2 window_size_i, 
                        window::CursorPosition cursor_pos
                    )
{
    renderer_3D.bind_window_fb(window_size_i);

	renderer_3D.render_scene_3d(    *window_scene, 
                                    *this           );

	renderer_3D.render_object_ids(  *window_scene,
                                    *this           );
    
}
