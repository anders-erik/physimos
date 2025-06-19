
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "math/vecmat.hh"
// #include "model_editor.hh"
#include "window/auxwin.hh"

// #include "rubik.hh"
struct GLFWWindow;




struct AxesVertex {
    f3 pos;
    f3 color;
};

/** Draw unit axes at scene origin. */
class ProgramAxes : protected opengl::Program
{
    unsigned int vao;
    unsigned int vbo;

    unsigned int transform_location;
    unsigned int view_location;
    unsigned int perspective_location;

public:

    ProgramAxes() : opengl::Program("3D/axes") {};

    void init();
    void set_uniforms(m4f4 transform_mat, m4f4 view_mat, m4f4 pers_mat);
    void render();
};

