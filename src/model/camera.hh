
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "math/math.hh"
#include "model_editor.hh"
#include "window/auxwin.hh"

// #include "rubik.hh"

namespace xpeditor {



struct CameraOrbital {
    Transform transform;
    
    m4f4 view_mat;
    m4f4 perspective_mat;


    float fov = 0.785f;
    float width = 600.0f;
    float height = 400.0f;
    float zn = 0.1f;
    float zf = 100.0f;

    float rho   = 11.0f; // Radius
    float theta = 0.83f; // x-y plane
    float phi   = 0.89f; // z-axis

    float rho_factor    = -0.2f;
    float theta_scale   = -0.008f;
    float phi_scale     =  0.010f;

    void rho_change(float delta);
    void theta_change(float delta);
    void phi_change(float delta);

    void set_triplet(float _rho, float _theta, float _phi);

    void set_matrices();
};


}