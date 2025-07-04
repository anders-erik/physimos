
#pragma once

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "scene/mesh.hh"



class ShaderColorLight : protected opengl::ShaderProgram
{

    unsigned int VAO;
    unsigned int vbo_pos;
    unsigned int vbo_norm;
    unsigned int EBO;

    uint view_mat_LOC;
    uint perspective_mat_LOC;

    uint light_pos_LOC;
    uint light_color_LOC;

public:

    ShaderColorLight() : ShaderProgram("3D/color_light") {};

    void init();

    void set_camera_view_perspective(m4f4 view_mat, m4f4 perspective_mat);
    void set_light_pos(f3 light_pos);
    void set_light_color(f3 light_color);

    void render(const m4f4& model_matrix, Mesh& mesh);

};



