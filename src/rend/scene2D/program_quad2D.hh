
#pragma once

#include <array>

#include "math/vecmat.hh"

#include "opengl/program.hh"





struct VertexQuad2D {
    f3 pos;
    f2 tex;
};

class ProgramQuad2D : protected opengl::Program {

    unsigned int camera2D_mat_LOC;
    unsigned int model_mat_LOC;

    unsigned int vao;
    unsigned int vbo;

public:

    ProgramQuad2D() : Program("2D/quad") {};

    void init();
    void set_camera_matrix(m3f3 _camera_matrix);

    void set_model_texture(const m3f3 model_matrix, const unsigned int texture) const;
    void draw() const;

    // Quad as two triangles of unit dimensions ( [0,1]x[0,1] ), thus center at (0.5, 0.5).
    static std::array<VertexQuad2D, 6> generate_quad_verts_c05();

};




