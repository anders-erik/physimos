#pragma once

#include "math/vecmat.hh"
#include "math/const.hh"
#include "math/transform.hh"
#include "math/quarternion.hh"
#include "math/spherical.hh"

#include "projection.hh"




struct OrbitalContext
{

    // float rho   =  10.0f; // Radius
    // float theta =  0.0f; // x-y plane
    // float phi   =  PIHf; // z-axis

    float rho   =  8.0f; // Radius
    float theta =  4.5f; // x-y plane
    float phi   =  1.0f; // z-axis
    // float rho   =  3.0f; // Radius
    // float theta =  -PIHf; // x-y plane
    // float phi   =  0.0f; // z-axis

    float rho_min       = 0.3f;
    float rho_max       = 60.0f;
    float phi_min       = 0.01f;
    float phi_max       = 3.13f;

    void set_rho(float new_rho);
    void set_theta(float new_theta);
    void set_phi(float new_phi);

    float rho_clamp(float _rho);
    float phi_clamp(float _phi);

    void  rho_scale(float factor);
    void  theta_add(float delta);
    void  phi_add(float delta);
};


struct GeoCamera
{
    // f3 position {10.0f, 10.0f, 10.0f}; 
    f3 position {0.0f, 0.0f, 0.0f}; 
    // Quarternion rotation {1.0f, 0.0f, 0.0f, 0.0f};

    m4f4 view;

    Projection projection;

    void init(int width, int height)
    {
        view = m4f4::I();
        
        projection.set_finite();
        projection.set_viewport(width, height);
    }

    void set_pos(f3 new_pos)
    {
        position = new_pos;
        reload_matrixes();
    }
    f3 get_pos()
    {
        return position;
    }
    // void set_rotation(Quarternion new_rotation)
    // {
    //     rotation = new_rotation;
    //     reload_matrixes();
    // }
    // Quarternion get_rotation()
    // {
    //     return rotation;
    // }


    f3 move(f3 pos_delta)
    {
        f3 new_pos = pos_delta + get_pos();
        set_pos(new_pos);
        return new_pos;
    }
    // void rotate(f3 axis, float angle)
    // {
    //     Quarternion new_rotation = rotation.rotate(axis, angle);
    //     set_rotation(new_rotation);
    //     reload_matrixes();
    // }


    // void set_orbit_center()
    // {
    //     f3_sphere spherical = Spherical::from_cart(position);
    //     orbit_ctx.set_rho(      spherical.get_rho()     );
    //     orbit_ctx.set_theta(    spherical.get_theta()   );
    //     orbit_ctx.set_phi(      spherical.get_phi()     );
    // }


    m4f4 reload_matrixes()
    {
        // m4f4 position_matrix;
        // position_matrix.x.w = -position.x;
        // position_matrix.y.w = -position.y;
        // position_matrix.z.w = -position.z;
        m4f4 translation_matrix = m4f4::translation(-position);

        // m4f4 rotation_matrix;
        
        // m4f4 align_x_rot = m4f4::rotation_x(-PIHf) * m4f4::rotation_z(-PIHf);
        // m4f4 rotation_matrix    =  align_x_rot * rotation.inverse().matrix();
        // m4f4 rotation_matrix;

        f3_sphere spherical = Spherical::from_cart(position);
        OrbitalContext orbit_ctx;
        orbit_ctx.set_rho(      spherical.get_rho()     );
        orbit_ctx.set_theta(    spherical.get_theta()   );
        orbit_ctx.set_phi(      spherical.get_phi()     );
        // f3_sphere.print("");

        Quarternion rotation {1.0f, 0.0f, 0.0f, 0.0f};
        rotation.reset();
        rotation.rotate(f3::Z(), -(PIf));
        rotation.rotate(f3::Y(), -(PIHf - orbit_ctx.phi));
        rotation.rotate(f3::Z(), -(-orbit_ctx.theta));


        m4f4 align_x_rot = m4f4::rotation_x(-PIHf) * m4f4::rotation_z(-PIHf);
        m4f4 rotation_matrix    =  align_x_rot * rotation.inverse().matrix();

        view = translation_matrix;
        // view = rotation_matrix * translation_matrix;
        // view = position_matrix * rotation_matrix;

        return view;
    }

};