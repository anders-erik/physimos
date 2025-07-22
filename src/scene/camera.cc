#include <cmath>
#include <iostream>

#include "math/spherical.hh"

#include "camera.hh"



m4f4& Projection::update_matrix()
{
    matrix.set_to_I();

    if(tag == Tag::Finite)
    {
        float ar = base.finite.AR;
        float tan_half_fov = tanf(base.finite.fov / 2.0f);
        float n = base.finite.zn;
        float f = base.finite.zf;

        matrix.x.x = 1 / (ar * tan_half_fov);
        matrix.y.y = 1 / tan_half_fov;
        matrix.z.z = - (f + n) / (f - n);
        matrix.w.w = 0.0f;

        matrix.z.w = - 2.0f * f * n / (f - n);
        matrix.w.z = - 1.0f;
    }
    else if(tag == Tag::Infinite)
    {
        float ar = base.finite.AR;
        float tan_half_fov = tanf(base.finite.fov / 2.0f);
        float n = base.infinite.zn;

        matrix.x.x = 1 / (ar * tan_half_fov);
        matrix.y.y = 1 / tan_half_fov;
        matrix.z.z = - 1;
        matrix.w.w = 0.0f;

        matrix.z.w = - 2.0f * n;
        matrix.w.z = - 1.0f;
    }
    else if(tag == Tag::Ortho)
    {
        float r = base.ortho.xmax;
        float t = base.ortho.ymax;
        float n = base.ortho.zn;
        float f = base.ortho.zf;

        matrix.x.x = 1 / r;
        matrix.y.y = 1 / t;
        matrix.z.z = 2 / (n-f);
        matrix.w.w = 1.0f;

        matrix.z.w = (f+n) / (n-f);
    }
    else 
    {
        throw "Updated camera projection matrix without a matching tag.";
    }

    
    return matrix;
}






m4f4& CameraView::calc_matrix(f3 pos, Quarternion rot)
{
    m4f4 align_x_rot = m4f4::rotation_x(-PIHf) * m4f4::rotation_z(-PIHf);

    m4f4 translation_matrix = m4f4::translation(-pos);
    m4f4 rotation_matrix    =  align_x_rot * rot.inverse().matrix();

    matrix = rotation_matrix * translation_matrix;
    return matrix;
}




void OrbitalContext::set_rho(float new_rho)
{
    rho = rho_clamp(new_rho);
}

void OrbitalContext::set_theta(float new_theta)
{
    theta = new_theta;
}

void OrbitalContext::set_phi(float new_phi)
{
    phi = phi_clamp(new_phi);
}

float OrbitalContext::
rho_clamp(float _rho)
{
    if(_rho < rho_min)
        return rho_min;
    if(_rho > rho_max)
        return rho_max;
    return _rho;
}


float OrbitalContext::
phi_clamp(float _phi)
{
    if(_phi < phi_min)
        return phi_min;
    if(_phi > phi_max)
        return phi_max;
    return _phi;
}


void OrbitalContext::
rho_scale(float factor)
{
    rho = rho_clamp(rho * factor);
}


void OrbitalContext::
theta_add(float delta)
{
    theta += delta;
}


void OrbitalContext::
phi_add(float delta)
{
    phi = phi_clamp(phi + delta);
}



void Camera::set_free()
{
    state.bits = CameraState::FREE;
    state.tag = TagO{};
}

void Camera::set_orbit_center()
{
    state.bits = CameraState::ORB_CENTER;
    state.tag = TagO{};

    // Maintain cam position 
    f3_sphere spherical = Spherical::from_cart(object.pos);
    view.orbit_ctx.set_rho(      spherical.get_rho()     );
    view.orbit_ctx.set_theta(    spherical.get_theta()   );
    view.orbit_ctx.set_phi(      spherical.get_phi()     );
}

void Camera::set_orbit_tag(TagO new_tag)
{
    state.bits = CameraState::ORB_TAG;
    state.tag = new_tag;

    // Maintain cam position 
    f3_sphere spherical = Spherical::from_cart(object.pos);
    view.orbit_ctx.set_rho(      spherical.get_rho()     );
    view.orbit_ctx.set_theta(    spherical.get_theta()   );
    view.orbit_ctx.set_phi(      spherical.get_phi()     );
}


void Camera::update_matrices()
{
    view.calc_matrix(object.pos, object.rot);
    projection.update_matrix();
}
