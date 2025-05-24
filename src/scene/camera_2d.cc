#include <iostream>

#include "phont/phont.hh"
#include "math/vecmat.hh"

#include "scene/camera_2d.hh"



namespace scene2D { 



void Box2D::print(){
    std::cout << "Box2D: " << std::endl;
    std::cout << "box.pos.x  = " << pos.x << std::endl;
    std::cout << "box.pos.y  = " << pos.y << std::endl;
    std::cout << "box.size.x = " << size.x << std::endl;
    std::cout << "box.size.y = " << size.y << std::endl;
    
}


void Camera2D::reload(){

    box.size.x = width_initial / zoom_current;
    box.size.y = box.size.x / aspect_ratio;

    units_per_px = box.size.x / window_size_px.x;

    transform.scale.x = zoom_current;
    transform.scale.y = transform.scale.x * aspect_ratio;

    transform.set_matrix_camera();
}

void Camera2D::set_window_size_px(f2 size){
    window_size_px.x = size.x;
    window_size_px.y = size.y;

    aspect_ratio = window_size_px.x / window_size_px.y;

    reload();
}

void Camera2D::set_zoom_multiplier(float multiplier){
    zoom_multiplier = multiplier;
}

void Camera2D::zoom_set(float zoom){
    zoom_current = zoom;
    reload();
}

void Camera2D::zoom(float delta){

    // only determine direction for now
    if(delta > 0)
        zoom_current *= zoom_multiplier;
    else
        zoom_current *= 1.0f / zoom_multiplier;

    reload();
    transform.set_matrix_camera();
}


void Camera2D::pan(f2 delta_px){

    box.pos.x -= units_per_px * delta_px.x;
    box.pos.y -= units_per_px * delta_px.y;

    transform.pos.x = box.pos.x;
    transform.pos.y = box.pos.y;
    transform.set_matrix_camera();
}





}
