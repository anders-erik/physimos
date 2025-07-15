
#pragma once

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "scene/mesh.hh"



class ShaderColorLightInst : protected opengl::ShaderProgram
{

    unsigned int VAO;
    unsigned int vbo_pos;
    unsigned int vbo_norm;
    unsigned int vbo_color;
    unsigned int vbo_model_mats;
    unsigned int EBO;

    uint tri_face_count = 0;

    uint view_mat_LOC;
    uint perspective_mat_LOC;

    uint light_pos_LOC;
    uint light_color_LOC;

    uint color_texunit_LOC;

public:

    ShaderColorLightInst() : ShaderProgram("3D/color_light_inst") {};

    void init();

    void set_camera_view_perspective(m4f4 view_mat, m4f4 perspective_mat);
    void set_light_pos(f3 light_pos);
    void set_light_color(f3 light_color);

    void set_data(Mesh& mesh);

    void render_fill(uint instance_count);

};



