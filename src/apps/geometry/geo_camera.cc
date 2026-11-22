#include <cmath>
#include <iostream>

#include "math/spherical.hh"

#include "geo_camera.hh"






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

