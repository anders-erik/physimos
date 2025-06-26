#include <cmath>
#include <iostream>

#include "math/spherical.hh"

#include "camera.hh"



void CameraPerspective::update_matrix()
{
    matrix.set_to_I();

    float aspect = width / height;
    float tan_half_fov = tanf(fov / 2.0f);

    matrix.x.x = 1 / (aspect * tan_half_fov);
    matrix.y.y = 1 / tan_half_fov;
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

float CameraPerspective::
AR()
{
    return width / height;
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
    rho = rho_clamp(rho + delta);
}


void OrbitalView::
theta_change(float delta)
{
    theta += delta;
}


void OrbitalView::
phi_change(float delta)
{
    phi = phi_clamp(phi + delta);
}



void OrbitalView::
update_matrix()
{
    // Camera position relative to rotational center
    f3 cam_center_offset = Spherical::to_cart({rho, theta, phi});
    // Camera position in scene coordinates
    f3 cam_pos = cam_center_offset + rotational_center;

    m4f4 T = m4f4::translation(-(cam_pos)); 

    // cam_pos.print("cam_pos");
    

    // rotation angles in parenthesis is desired scene behavior. Camera rotation is then the reverse

    /** Pre-rotate world by -90deg, making x-axis point downward on screen.  
        Thus for theta = 0 and extrinsic x-rotation this means camera descending towards +x. */
    m4f4 RZ_90 = m4f4::rotation_z(-(-1.57f)); 
    /** Rotate about z-axis, which is out of monitor by default. Before rotation: +x is down, +y is right. */
    m4f4 R_TH = m4f4::rotation_z(-(-theta));
    /** Angle from positive z-axis, which is out of monitor by default, towards negative z-axis. */
    m4f4 R_PHI = m4f4::rotation_x(-(phi));

    // EXTRINSIC
    matrix = R_PHI * R_TH * RZ_90 * T;
}



void CameraOrbital::
update()
{
    view.update_matrix();
    perspective.update_matrix();
}


f3 CameraOrbital::
get_forward()
{
    f3 center_to_cam = Spherical::to_cart({ view.rho, 
                                            view.theta, 
                                            view.phi});
    return (-center_to_cam).unit();
}


f3 CameraOrbital::
get_pos()
{
    return view.rotational_center +  Spherical::to_cart({   view.rho, 
                                                            view.theta, 
                                                            view.phi});
}


f3 CameraOrbital::
get_right()
{
    f3 center_to_cam = Spherical::to_cart({ view.rho, 
                                            view.theta, 
                                            view.phi});

    f3 projected_xy = {center_to_cam.to_xy(), 0.0f};

    f3 cam_xyprojected_cross = center_to_cam.cross(projected_xy);

    f3 cam_xyprojected_cross_unit = cam_xyprojected_cross.unit();

    if(view.phi < PIHf)
        return cam_xyprojected_cross_unit;
    else
        return -cam_xyprojected_cross_unit;
}

f3 CameraOrbital::
get_up()
{
    return get_right().cross(get_forward()).unit();
}


void CameraOrbital::
forward(float delta)
{
    view.rho_change(-delta);
}
void CameraOrbital::
backward(float delta)
{
    view.rho_change(delta);
}

void CameraOrbital::
left(float delta)
{
    view.theta_change(-delta);
}

void CameraOrbital::
right(float delta)
{
    view.theta_change(delta);
}

void CameraOrbital::
up(float delta)
{
    view.phi_change(-delta);
}

void CameraOrbital::
down(float delta)
{
    view.phi_change(delta);
}


void CameraOrbital::
print()
{
    std::cout << "Camera:" << std::endl;
    std::cout << "rho   = " << view.rho << std::endl;
    std::cout << "theta = " << view.theta << std::endl;
    std::cout << "phi   = " << view.phi << std::endl;
}