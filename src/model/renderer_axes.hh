
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "math/math.hh"
#include "model_editor.hh"
#include "window/auxwin.hh"

// #include "rubik.hh"
struct GLFWWindow;

namespace xpeditor {



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

}