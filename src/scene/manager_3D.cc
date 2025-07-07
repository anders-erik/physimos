#include <iostream>
#include <list>

#include "lib/log.hh"

#include "math/spherical.hh"
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

            
            phy->aabb.set_with_half_size(   object.pos, 
                                            phy->model_size * object.scale * 0.5f );
            phy->colliding = false;
        }
    }
    // Check collisions
    for(auto& phy_A : manager_p.physicss)
    {
        // AABBf aabb_A = {phy_A.YY.aabb.pos, phy_A.YY.aabb.size};
        
        for(auto& phy_B : manager_p.physicss)
        {
            if(phy_A.XX.pid == phy_B.XX.pid) continue;

            // AABBf aabb_B = {phy_B.YY.aabb.pos, phy_B.YY.aabb.size};

            bool collided = AABBf::collide(phy_A.YY.aabb, phy_B.YY.aabb);
            if(collided)
            {
                phy_A.YY.colliding = true;
                phy_B.YY.colliding = true;
            }
        }
    }


    // CAMERA
    CameraObject& camobj    = scenew.camobj;
    CameraView& cam         = scenew.camobj.view;
    Object&     c_obj       = scenew.camobj.object;

    f3 cam_f = Quarternion::rotate_f3(c_obj.rot, f3::X()).unit();
    f3 cam_l = Quarternion::rotate_f3(c_obj.rot, f3::Y()).unit();
    f3 cam_u = Quarternion::rotate_f3(c_obj.rot, f3::Z()).unit();


    Object* tag_obj = nullptr;
    f3 reference_pos = {0.0f, 0.0f, 0.0f}; // default is scene origin
    if(camobj.state.has_tag())
    {
        tag_obj = manager_o.get_object(camobj.state.tag);
        if(tag_obj == nullptr)
            camobj.set_free();
        else
            reference_pos = tag_obj->pos;
    }


    if(camobj.state.is_orbit())
    {
        float rho_factor = 1.0f;
        if(state.camera.deltas.z < 0.0f)
            rho_factor =  1.2f;
        else if(state.camera.deltas.z > 0.0f)
            rho_factor =  0.8f;


        // NOTE: Update angles BEFORE position.
        cam.orbit_ctx.rho_scale(  rho_factor                      );
        cam.orbit_ctx.theta_add( -state.camera.deltas.x  / 200.0f );
        cam.orbit_ctx.phi_add(    state.camera.deltas.y  / 400.0f );

        f3 cam_center_offset = Spherical::to_cart({ cam.orbit_ctx.rho, 
                                                    cam.orbit_ctx.theta, 
                                                    cam.orbit_ctx.phi     });
        
        c_obj.pos = cam_center_offset + reference_pos;

        // Full rotation calculation to match spherical values
        c_obj.rot.reset();
        c_obj.rot.rotate(f3::Z(), -(PIf));
        c_obj.rot.rotate(f3::Y(), -(PIHf - cam.orbit_ctx.phi));
        c_obj.rot.rotate(f3::Z(), -(-cam.orbit_ctx.theta));

    }
    else
    {
        float free_pan_factor = 2.0f;
        c_obj.rot.rotate(cam_l,    -state.camera.deltas.y 
                                    * free_pan_factor  
                                    / camobj.perspective.height);
        c_obj.rot.rotate(cam_u,    -state.camera.deltas.x 
                                    * free_pan_factor  
                                    / camobj.perspective.width);

        // remove tilt
        f3 HL = f3::Z().cross(cam_f); // horizontal left
        float angle_hl_l = HL.angle(cam_l); // absolute value angle

        // Check angle direction
        if(HL.z > cam_l.z)
            c_obj.rot.rotate(cam_f, angle_hl_l);
        else
            c_obj.rot.rotate(cam_f, -angle_hl_l);

        float move_factor = 0.2;
        if(state.camera.is_forward())
            c_obj.pos +=  cam_f * move_factor;
        if(state.camera.is_backward())
            c_obj.pos -=  cam_f * move_factor;
        if(state.camera.is_left())
            c_obj.pos +=  cam_l * move_factor;
        if(state.camera.is_right())
            c_obj.pos -=  cam_l * move_factor;
        if(state.camera.is_up())
            c_obj.pos +=  cam_u * move_factor;
        if(state.camera.is_down())
            c_obj.pos -=  cam_u * move_factor;
        
        c_obj.pos +=  cam_f * state.camera.deltas.z;
    }

    
    state.camera.deltas.set_zero();




    // SELECTED OBJECT

    Object* selected_o = manager_o.get_object(state.selected.tag);
    if(selected_o != nullptr)
    {

        // 1. TRANSLATE
        if(!state.selected.pos_delta.is_zero())
        {
            // SCALE MOVEMENT
            f3 cam_to_obj_delta = c_obj.pos - selected_o->pos;
            float obj_distance  = cam_to_obj_delta.norm();

            float tan_fov = tanf(camobj.perspective.fov);
            float view_width_at_obj_dist = obj_distance * tan_fov;

            // std::cout << obj_distance << ", " << view_width_at_obj_dist << std::endl;
            

            selected_o->pos -= cam_l * state.selected.pos_delta.x * view_width_at_obj_dist * camobj.perspective.AR();
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
            selected_o->rot.rotate(cam_l,  angle_delta.y);
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
