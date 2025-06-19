
#pragma once

#include "math/vecmat.hh"
#include "model/model.hh"

#include "opengl/program.hh"

#include "scene/mesh.hh"



class ProgramMesh : protected opengl::Program
{

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:

    ProgramMesh() : Program("3D/mesh") {};

    void init();

    void set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat);

    void render(Mesh& mesh, unsigned int color);

};



