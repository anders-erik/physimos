
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "math/math.hh"
#include "model_editor.hh"
#include "window/auxwin.hh"

// #include "rubik.hh"

namespace xpeditor {

struct GLFWWindow;


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
    void set_triplet(float _rho, float _theta, float _phi);

    void set_matrices();
};

struct AxesVertex {
    f3 pos;
    f3 color;
};

struct RendererAxes {
    opengl::Programs renderer_program = opengl::Programs::Axes;

    unsigned int vao;
    unsigned int vbo;

    unsigned int transform_location;
    unsigned int view_location;
    unsigned int perspective_location;

    void init();
    void set_uniforms(m4f4 transform_mat, m4f4 view_mat, m4f4 pers_mat);
    void render();
};

bool scene_init();

void scene_handle_input(window::InputState input_state);
void scene_update();
void scene_render();

void scene_set_viewport_dims(int _width, int _height);

}