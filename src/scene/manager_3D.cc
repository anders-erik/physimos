#include <iostream>
#include <list>

#include "lib/log.hh"

#include "math/quarternion.hh"

#include "rend/rend.hh"

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
    Scene3D& scenew = *window_scene;

    // COLLISION

    // Update bounding box before collision-test
    for(auto& object : manager_o.objects)
    {
        if(object.pyh_tag.pid != 0)
        {
            Physics* phy = manager_p.find_physics(object.pyh_tag);
            if(phy == nullptr) continue;

            phy->pos = object.pos;
            phy->colliding = false;
        }
    }
    // Check collisions
    for(auto& phy_A : manager_p.physicss)
    {
        AABBf aabb_A = phy_A.YY.aabb_base + phy_A.YY.pos;

        for(auto& phy_B : manager_p.physicss)
        {
            if(phy_A.XX.pid == phy_B.XX.pid) continue;

            AABBf aabb_B = phy_B.YY.aabb_base + phy_B.YY.pos;

            bool collided = AABBf::collide(aabb_A, aabb_B);
            if(collided)
            {
                phy_A.YY.colliding = true;
                phy_B.YY.colliding = true;
            }
        }
    }


    Object*     cam_o    = nullptr;
    CameraFree* cam_free = nullptr;
    for(auto tago : scenew.tagos)
    {
        if(tago.is_camera())
        {
            cam_o = manager_o.get_object(tago);
            if(cam_o == nullptr) continue;

            cam_free = manager_p.find_camera(cam_o->tagp);
            if(cam_free == nullptr) continue;

            break; // break if confirmed camera object AND camera property
        }
    }
    if(cam_o != nullptr && cam_free != nullptr)
    {
        f3 cam_f = Quarternion::rotate_f3(cam_o->rot, f3::X()).unit();
        f3 cam_l = Quarternion::rotate_f3(cam_o->rot, f3::Y()).unit();
        f3 cam_u = Quarternion::rotate_f3(cam_o->rot, f3::Z()).unit();

        float move_factor = 0.2;

        if(state.camera.is_forward())
            cam_o->pos +=  cam_f * move_factor;
        if(state.camera.is_backward())
            cam_o->pos -=  cam_f * move_factor;
        if(state.camera.is_left())
            cam_o->pos +=  cam_l * move_factor;
        if(state.camera.is_right())
            cam_o->pos -=  cam_l * move_factor;
        if(state.camera.is_up())
            cam_o->pos +=  cam_u * move_factor;
        if(state.camera.is_down())
            cam_o->pos -=  cam_u * move_factor;

        cam_o->pos +=  cam_f * state.camera.spherical_delta.x;

        cam_o->rot.rotate(cam_u, -state.camera.spherical_delta.y  / 400.0f);
        cam_o->rot.rotate(cam_l, -state.camera.spherical_delta.z  / 400.0f);

        // remove tilt
        f3 HL = f3::Z().cross(cam_f); // horizontal left
        float angle_hl_l = HL.angle(cam_l); // absolute value angle

        // Check angle direction
        if(HL.z > cam_l.z)
            cam_o->rot.rotate(cam_f, angle_hl_l);
        else
            cam_o->rot.rotate(cam_f, -angle_hl_l);

        state.camera.spherical_delta.set_zero();
        return;
    }

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
