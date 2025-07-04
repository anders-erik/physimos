
#pragma once

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "scene/mesh.hh"



class ShaderMesh : protected opengl::ShaderProgram
{

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:

    ShaderMesh() : ShaderProgram("3D/mesh") {};

    void init();

    void set_camera_view_projection(m4f4 persective_mat, m4f4 view_mat);
    /** alpha == 1.0f */
    void set_color(f3 color);

    void render(Mesh& mesh, unsigned int color);
    void render(const m4f4& model_matrix, Mesh& mesh, unsigned int color);
    void render(const m4f4& model_matrix, Mesh& mesh);
    void render_filled(const m4f4& model_matrix, Mesh& mesh);

};



