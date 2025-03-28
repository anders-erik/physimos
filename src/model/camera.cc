#include "glad/glad.h"
#include "camera.hh"

#include "model.hh"
#include "math/math.hh"

// #include "rubik.hh"

#include <iostream>
#include <cmath>

namespace xpeditor {

void CameraOrbital::rho_change(float delta){
    rho = rho + rho * rho_factor * delta;
    set_triplet(rho, theta, phi);
}
void CameraOrbital::theta_change(float delta){
    theta = theta + theta_scale * delta;
    set_triplet(rho, theta, phi);
}
void CameraOrbital::phi_change(float delta){
    phi = phi + phi_scale * delta;
    set_triplet(rho, theta, phi);
}

void CameraOrbital::set_triplet(float _rho, float _theta, float _phi){

    rho = _rho < 4.0f ? 4.0f : _rho ;
    rho = rho > 30.0f ? 30.0f : rho ;

    theta = _theta;

    // phi = _phi < 0.0f ? 0.0f : _phi;
    // phi = phi > 3.14f ? 3.14f : phi;
    phi = _phi;

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



}
