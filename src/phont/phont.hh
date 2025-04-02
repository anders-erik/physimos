
#pragma once

#include <array>
#include <vector>

#include "math/vecmat.hh"
#include "math/transform.hh"

#include "opengl/texture.hh"
#include "opengl/renderer_quad_2d.hh"


namespace phont {


struct GlyphMesh {
    std::vector<f3> verts;
    std::vector<i3> faces;
};


void set_texture_checker(unsigned int& texture_id);

opengl::TextureFrameBuffer get_texture_F();
void get_mesh_F(GlyphMesh& mesh);


}
