#pragma once

#include <cmath>

#include "math/const.hh"
#include "math/vecmat.hh"
#include "transform.hh"

/** Not used */
struct f3_sphere : public f3
{
    float& get_rho()    { return x; }
    float& get_theta()  { return y; }
    float& get_phi()    { return z; }
};


/** Rho  :  distance from origin
    Theta:  CCW about Z-axis from +X axis [0, 2PI]
    Phi  :  from +Z axis [rad], [0, PI]
*/
struct Spherical
{
    static f3 to_cart(f3 spher)
    {
        float rho   = spher.x;
        float theta = spher.y;
        float phi   = spher.z;

        return {    rho * sinf(phi) * cosf(theta),
                    rho * sinf(phi) * sinf(theta),
                    rho * cosf(phi)                 };
    }
    static f3 to_cart(float rho, float theta, float phi)
    {
        return {    rho * sinf(phi) * cosf(theta),
                    rho * sinf(phi) * sinf(theta),
                    rho * cosf(phi)                 };
    }

    static f3_sphere from_cart(f3 cart)
    {
        f2 xy = cart.to_xy().clamp_nonzero();

        float rho   = cart.norm();
        /** Rotation in x-y plane */
        float theta = xy.angle();
        /** as phi=0 at vertical we subtract horizontal pitch value */
        float phi   = PIHf - atanf( cart.z / xy.norm());

        return (f3_sphere)f3{rho, theta, phi};
    }
};