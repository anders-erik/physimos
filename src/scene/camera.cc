#include "glad/glad.h"
#include "camera.hh"

#include "model.hh"
#include "math/vecmat.hh"

// #include "rubik.hh"
// #include "renderer_model.hh"

#include <iostream>
#include <cmath>



CameraOrbital::CameraOrbital(){
    triplet_reload();
}

void CameraOrbital::set_fov(int _width, int _height){
    width = (float) _width;
    height = (float) _height;
}


float CameraOrbital::rho_clamp(float _rho){
    if(_rho < rho_min)
        return rho_min;
    if(_rho > rho_max)
        return rho_max;
    return _rho;
}
void CameraOrbital::rho_change(float delta){
    float rho_new = rho + rho * rho_factor * delta;

    // rho = rho_new < 4.0f ? 4.0f : rho_new ;
    // rho = rho > 30.0f ? 30.0f : rho ;

    triplet_set(rho_clamp(rho_new), theta, phi);

    // triplet_set(rho, theta, phi);
}
void CameraOrbital::theta_change(float delta){
    theta = theta + theta_scale * delta;
    triplet_set(rho, theta, phi);
}
void CameraOrbital::phi_change(float delta){
    phi = phi + phi_scale * delta;
    triplet_set(rho, theta, phi);
}

void CameraOrbital::triplet_set(float _rho, float _theta, float _phi){

    // rho = _rho < 4.0f ? 4.0f : _rho ;
    // rho = rho > 30.0f ? 30.0f : rho ;

    rho = rho_clamp(_rho);
    theta = _theta;
    phi = _phi;

    // phi = _phi < 0.0f ? 0.0f : _phi;
    // phi = phi > 3.14f ? 3.14f : phi;

    triplet_reload();
}

void CameraOrbital::triplet_reload(){
    // triplet_set(rho, theta, phi);

    transform.pos.x = rho * cosf(theta) * sinf(phi);
    transform.pos.y = rho * sinf(theta) * sinf(phi);
    transform.pos.z = rho * cosf(phi);

    transform.rot.x = phi;
    transform.rot.z = 1.57f + theta; // Offset by 90 deg to align camera with initial theta

}

void CameraOrbital::set_matrices(){
    m4f4 indentity;

    view_mat = indentity;

    view_mat.rotate_x(-transform.rot.x);
    view_mat.rotate_y(-transform.rot.y);
    view_mat.rotate_z(transform.rot.z);

    f3 negative_camera_pos;
    negative_camera_pos.x = -transform.pos.x;
    negative_camera_pos.y = -transform.pos.y;
    negative_camera_pos.z = -transform.pos.z;
    view_mat.translate(negative_camera_pos);


    perspective_mat = indentity;
    perspective_mat.perspective(
                        fov, 
                        width, 
                        height, 
                        zn,
                        zf
    );

}



