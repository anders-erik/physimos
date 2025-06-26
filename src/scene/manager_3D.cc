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

        // SCALE MOVEMENT
        f3 cam_pos          = window_scene->camera.get_pos();
        f3 cam_to_obj_delta = cam_pos - selected_o->pos;
        float tan_half_fov = tanf(camera.perspective.fov / 2.0f);
        float view_width_at_obj_dist = 2.0f * tan_half_fov * cam_to_obj_delta.norm();


        selected_o->pos += cam_r * state.selected.pos_delta.x * view_width_at_obj_dist * camera.perspective.AR();
        selected_o->pos += cam_u * state.selected.pos_delta.y * view_width_at_obj_dist;
        selected_o->pos += cam_f * state.selected.pos_delta.z;

        state.selected.pos_delta.set_zero();



        // 2. ROTATE
        if(!state.selected.rot_delta_norm.is_zero())
        {
            
        float angle = state.selected.rot_delta_norm.norm() * 10.0f;

        f3 move_axis_x;
        f3 move_axis_y;
        if(state.selected.rot_delta_norm.x != 0.0f)
            move_axis_x = (cam_r * state.selected.rot_delta_norm.x).unit();
        if(state.selected.rot_delta_norm.y != 0.0f)
            move_axis_y = (cam_u * state.selected.rot_delta_norm.y).unit();
        f3 move_axis = (move_axis_x + move_axis_y).unit();

        f3 rot_axis = move_axis.cross(cam_f).unit();

        Quarternion rot_q = {0.0f, rot_axis};
        selected_o->rot = Quarternion::rotate_quart(selected_o->rot, rot_q, angle);
        // rot_q.set_angle(angle);
        
        // selected_o->rot = rot_q.rotate(selected_o->rot);
        // std::cout << selected_o->rot.x << std::endl;
        
        // state.selected.rot_delta_norm.print("rot_delta");
        state.selected.rot_delta_norm.set_zero();

        // Quarternion::conjugate
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
