
#pragma once

#include "math/vecmat.hh"

#include "opengl/texture.hh"
#include "opengl/program.hh"

#include "model/model.hh"



struct Mesh;


class ProgramQuad : protected opengl::Program
{
    unsigned int vao;

    unsigned int vbo_vert;
    unsigned int vbo_text;

    unsigned int ebo;

    int model_matrix_LOC;
    int view_matrix_LOC;
    int projection_matrix_LOC;
    int texture_0_LOC;

    const unsigned int vertex_count = 6;

public:

    ProgramQuad() : Program("3D/quad") {};

    void init();

    void set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat);

    /** Recreates render context on every call. */
    void render(const m4f4& model_matrix, Mesh& mesh, unsigned int texture_id);

};




