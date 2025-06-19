
#pragma once

#include <array>
#include <vector>

#include "math/vecmat.hh"

#include "opengl/program.hh"




class ProgramVector : protected opengl::Program {

    unsigned int view_mat_LOC;
    unsigned int project_mat_LOC;

    unsigned int vao;
    unsigned int vbo;

public:

    ProgramVector() : Program("3D/vector") {};

    std::vector<f3> arrow;

    float scale = 4.0f;
    f3 color = f3(1.0f, 1.0f, 1.0f);

    void set_project_view_matrix(m4f4 project_mat, m4f4 view_mat);
    void render(f3 pos, f3 size);
    void init();

};





