#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "scene/camera2D.hh"
#include "camera2D.hh"



namespace scene { 




m3f3 Camera2D::get_matrix()
{
    m3f3 M_s_ndc; // Complete matrix from scene coords to ndc

    
    box.size.y = box.size.x / AR();

    // scene -> camera/view
    m3f3 t_M_s_c;   // translate: scene to camera box
    t_M_s_c.x.z = -box.pos.x;
    t_M_s_c.y.z = -box.pos.y;

    m3f3 s_M_c_ndc; // scale: camera to NDC
    s_M_c_ndc.x.x = 2 / box.size.x;
    s_M_c_ndc.y.y = 2 * AR() / box.size.x;

    m3f3 t_M_c_ndc; // translate: camera to NDC
    t_M_c_ndc.x.z = -1.0f;
    t_M_c_ndc.y.z = -1.0f;

    M_s_ndc = t_M_c_ndc * s_M_c_ndc * t_M_s_c;

    return M_s_ndc;
}

void Camera2D::set_framebuffer_size(f2 size)
{
    framebuffer_size_scene = size;
    reload_height();
}

void Camera2D::reload_height()
{
    box.size.y = box.size.x / AR();
}

void Camera2D::set_width(float new_width)
{
    box.size.x = new_width;
    reload_height();
}

float Camera2D::get_width()
{
    return box.size.x;
}

float Camera2D::AR()
{
    return framebuffer_size_scene.x / framebuffer_size_scene.y;
}


void Camera2D::pan(f2 delta_scene)
{
    // Subtract: I want to experience `dragging` the 2D scene.
    // Therefore a mouse movement to the left should push the camera to the right in the scene.
    box.pos -= delta_scene;
}


f2 Camera2D::normalized_to_scene_coords(f2 normalized)
{
    return box.to_box(normalized);
}

f2 Camera2D::normalized_to_scene_delta(f2 normalized)
{
    return box.to_box_delta(normalized);
}

Box2D Camera2D::get_box()
{
    return box;
}




}
