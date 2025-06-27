#pragma once

#include "math/const.hh"
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
    float AR();
    
    void update_matrix();
};



struct OrbitalView
{
    f3 rotational_center;   // the point about which the camera is orbiting
    m4f4 matrix;            // Full view matrix. Set by calling 'update_matrix()'

    float rho   =  8.0f; // Radius
    float theta =  4.5f; // x-y plane
    float phi   =  1.0f; // z-axis
    // float rho   =  3.0f; // Radius
    // float theta =  -PIHf; // x-y plane
    // float phi   =  0.0f; // z-axis

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

    f3 get_pos();
    f3 get_forward();
    f3 get_right();
    f3 get_up();

    void forward(float delta);
    void backward(float delta);
    void left(float delta);
    void right(float delta);
    void up(float delta);
    void down(float delta);

    void print();
};


