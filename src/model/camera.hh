
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "math/vecmat.hh"
// #include "model_editor.hh"
#include "renderer_model.hh"
#include "window/auxwin.hh"

// #include "rubik.hh"

namespace xpeditor {



class CameraOrbital {
    float rho_factor    = -0.2f;
    float theta_scale   = -0.008f;
    float phi_scale     =  0.010f;

    float rho_min       = 0.5f;
    float rho_max       = 20.0f;

    float fov = 0.785f;
    float width = 600.0f;
    float height = 400.0f;
    float zn = 0.1f;
    float zf = 100.0f;
    
public:
    Transform transform;
    
    m4f4 view_mat;
    m4f4 perspective_mat;

    float rho   = 11.0f; // Radius
    float theta = 0.83f; // x-y plane
    float phi   = 0.89f; // z-axis

    

    CameraOrbital();

    void set_fov(int width, int height);

    void rho_change(float delta);
    void theta_change(float delta);
    void phi_change(float delta);

    float rho_clamp(float _rho);

    void triplet_reload();
    void triplet_set(float _rho, float _theta, float _phi);

    void set_matrices();
};


}