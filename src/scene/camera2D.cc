#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "scene/camera2D.hh"



namespace scene { 




void Camera2D::matrix_reload(){

    // Reload values dependent on window size and box.x
    AR = window_size_px.x / window_size_px.y;
    box.size.y = box.size.x / AR;

    // scene -> camera/view
    t_M_s_c.x.z = -box.pos.x;
    t_M_s_c.y.z = -box.pos.y;

    // camera/view -> ndc
    t_M_c_ndc.x.z = -1.0f;
    t_M_c_ndc.y.z = -1.0f;
    s_M_c_ndc.x.x = 2 / box.size.x;
    s_M_c_ndc.y.y = 2 * AR / box.size.x;


    M_s_ndc.set_to_identity();

    M_s_ndc.mult_left(t_M_s_c);

    M_s_ndc.mult_left(s_M_c_ndc);
    // | order of these two? |
    M_s_ndc.mult_left(t_M_c_ndc);

}

m3f3 Camera2D::get_matrix(){
    return M_s_ndc;
}

void Camera2D::set_window_size_px(f2 size){
    window_size_px = size;
    matrix_reload();
}

void Camera2D::set_zoom_factor(float factor){
    zoom_factor = factor;
}

void Camera2D::set_width(float zoom){
    box.size.x = zoom;
    matrix_reload();
}

void Camera2D::zoom(float delta){

    // only determine direction - each scroll event is equivalent distance
    if(delta < 0)
        box.size.x *= zoom_factor; // zoom out by increasing camera width
    else
        box.size.x *= 1.0f / zoom_factor;

    matrix_reload();
}

void Camera2D::pan(f2 delta_sane){

    box.pos.x -= delta_sane.x;
    box.pos.y -= delta_sane.y;

    matrix_reload();
}

Box2D Camera2D::get_box(){
    return box;
}

void Camera2D::print(){

    box.print();

}



}
