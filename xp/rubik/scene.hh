
#pragma once

#include <vector>

#include "gpu.hh"
#include "math.hh"
#include "model.hh"
#include "window.hh"

#include "rubik.hh"

namespace xprubik {

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

    void orbit_z(float orbit_z_delta);
    void orbit_xy(float xy_delta);
    void zoom(float zoom_delta);

    void set_matrices();
};

struct AxesVertex {
    f3 pos;
    f3 color;
};

struct RendererAxes {
    Shader renderer_program = Shader::Axes;

    unsigned int vao;
    unsigned int vbo;

    unsigned int transform_location;
    unsigned int view_location;
    unsigned int perspective_location;

    void init();
    void set_uniforms(m4f4 transform_mat, m4f4 view_mat, m4f4 pers_mat);
    void render();
};

bool scene_init(Cube& _cube);

void scene_handle_input(InputState input_state);
void scene_update(Cube& _cube);
void scene_render(Cube& _cube);

void scene_set_viewport_dims(int _width, int _height);

}