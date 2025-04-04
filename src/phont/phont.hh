
#pragma once

#include <array>
#include <vector>

#include "math/vecmat.hh"
#include "math/transform.hh"

#include "opengl/texture.hh"
#include "opengl/renderer_quad_2d.hh"


namespace phont {

struct GlyphRenderContext {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    int vertex_count;
};

struct GlyphMesh {
    std::vector<f3> verts;
    std::vector<i3> faces;
};

struct GlyphTextureGenerator {
    char char_value = 0x0;
    GlyphMesh mesh_glyph;
    GlyphRenderContext render_context;
    opengl::TextureFrameBuffer text_framebuff;

    GlyphTextureGenerator(char ch, i2 size);
    // GlyphTextureGenerator() = default;

    void set_char(char ch);
    void set_texture_size(i2 size);
    opengl::Texture get_texture();
    void generate();
};

void get_mesh_F(GlyphMesh& mesh);
void get_mesh_A(GlyphMesh& mesh);


}
