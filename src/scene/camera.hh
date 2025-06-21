#pragma once


#include "math/transform.hh"


struct CameraPerspective
{
    m4f4 matrix; // Full perspective matrix. Set by calling 'update_matrix()'

    float fov = 0.785f;
    float width = 600.0f;
    float height = 400.0f;
    float zn = 0.1f;
    float zf = 100.0f;

    void set_fov(int new_width, int new_height);
    
    void update_matrix();
};



struct OrbitalView
{
    f3 center_pos;  // the point about which the camera is orbiting
    m4f4 matrix;    // Full view matrix. Set by calling 'update_matrix()'

    float rho   =  3.0f; // Radius
    float theta =  4.5f; // x-y plane
    float phi   =  1.0f; // z-axis
    // float rho   =  0.0f; // Radius
    // float theta =  0.0f; // x-y plane
    // float phi   =  0.0f; // z-axis

    float rho_factor    = -0.2f;
    float theta_scale   = -0.004f;
    float phi_scale     =  0.005f;

    float rho_min       = 0.5f;
    float rho_max       = 20.0f;
    float phi_min       = 0.01f;
    float phi_max       = 3.13f;


    float rho_clamp(float _rho);
    float phi_clamp(float _phi);

    void rho_change(float delta);
    void theta_change(float delta);
    void phi_change(float delta);

    void update_matrix();
};





struct CameraOrbital
{
    CameraPerspective perspective;
    OrbitalView view;

    /** Makes view and perspective matrices reflect most recent values. */
    void update();
};


