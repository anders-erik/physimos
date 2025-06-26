
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
class ShaderAxes : protected opengl::ShaderProgram
{
    unsigned int vao;
    unsigned int vbo;

    unsigned int model_matrix_LOC;
    unsigned int view_location;
    unsigned int perspective_location;

public:

    ShaderAxes() : opengl::ShaderProgram("3D/axes") {};

    void init();
    void set_uniforms(m4f4 transform_mat, m4f4 view_mat, m4f4 pers_mat);
    void render();
    void render(m4f4 model_mat);
};

