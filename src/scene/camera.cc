
#include <cmath>

#include "camera.hh"



void CameraPerspective::update_matrix()
{
    matrix.set_to_I();

    float aspect = width / height;
    float tanHalfFov = tanf(fov / 2.0f);

    matrix.x.x = 1 / (aspect * tanHalfFov);
    matrix.y.y = 1 / tanHalfFov;
    matrix.z.z = - (zf + zn) / (zf - zn);
    matrix.w.w = 0.0f;

    matrix.z.w = - (2.0f * zf * zn) / (zf - zn);
    matrix.w.z = - 1.0f;
}

void CameraPerspective::set_fov(int new_width, int new_height)
{
    width  = (float) new_width;
    height = (float) new_height;
    update_matrix();
}






float OrbitalView::
rho_clamp(float _rho)
{
    if(_rho < rho_min)
        return rho_min;
    if(_rho > rho_max)
        return rho_max;
    return _rho;
}


float OrbitalView::
phi_clamp(float _phi)
{
    if(_phi < phi_min)
        return phi_min;
    if(_phi > phi_max)
        return phi_max;
    return _phi;
}


void OrbitalView::
rho_change(float delta)
{
    rho = rho_clamp(rho + rho * rho_factor * delta);
}


void OrbitalView::
theta_change(float delta)
{
    theta = theta + theta_scale * delta;
}


void OrbitalView::
phi_change(float delta)
{
    phi = phi_clamp(phi + phi_scale * delta);
}



void OrbitalView::
update_matrix()
{
    // cameras position relative to center -- using Spherical Coordinates
    f3 cam_center_offset = {    rho * sinf(phi) * cosf(theta),
                                rho * sinf(phi) * sinf(theta),
                                rho * cosf(phi)                 };
    // Camera position in scene coordinates 
    f3 cam_pos = cam_center_offset + center_pos;
    // Translate world objects a negative pos amount
    m4f4 T = Transform::create_translation(-cam_pos); 

    // TODO: change the below rotation calls to a 'look_at' call. At present these transforms are not intuitive

    /** Pre-rotate world by -90deg making phi rotation match spherical position. 
        For theta = 0 this means camera is moving towards +x. */
    m4f4 RZ_90 = Transform::create_rotation_z(1.57f); 
    /** Rotate about z-axis, which is out of monitor by default. Before rotation: +x is down, +y is right. */
    m4f4 R_TH = Transform::create_rotation_z(theta);
    /** Angle from positive z-axis, which is out of monitor by default, towards negative z-axis. */
    m4f4 R_PHI = Transform::create_rotation_x(-phi);

    matrix = R_PHI * R_TH * RZ_90 * T;
}



void CameraOrbital::
update()
{
    view.update_matrix();
    perspective.update_matrix();
}