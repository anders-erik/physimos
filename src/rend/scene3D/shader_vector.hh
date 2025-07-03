
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"




class ShaderVector : protected opengl::ShaderProgram {

    unsigned int view_mat_LOC;
    unsigned int project_mat_LOC;

    unsigned int vao;
    unsigned int vbo;

public:

    ShaderVector() : ShaderProgram("3D/vector") {};

    std::vector<f3> arrow;

    float scale = 4.0f;
    f3 color = f3(1.0f, 1.0f, 1.0f);

    void set_project_view_matrix(m4f4 project_mat, m4f4 view_mat);
    void set_color(f3 new_color);
    void render(f3 pos, f3 vector);
    void init();

};





