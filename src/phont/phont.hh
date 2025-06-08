
#pragma once

#include <array>
#include <vector>
#include <map>

#include "math/vecmat.hh"
#include "math/transform.hh"

#include "opengl/texture.hh"
#include "scene/render/renderer2D.hh"


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

struct Glyph {
    // New dimensions based on FreeType
    i2 pos;
    i2 size;
    float advance;

    // Glyph
    char char_value = 0x0;
    GlyphMesh mesh_glyph;
    GlyphRenderContext render_context;
    opengl::TextureFrameBuffer text_framebuff;

    opengl::TextureFrameBufferMultisample text_framebuff_multi;

    Glyph(char ch, i2 size);
    // GlyphTextureGenerator() = default;

    void set_char(char ch);
    void set_texture_size(i2 size);
    opengl::Texture get_texture();
    void generate();
    void generate_multisample();
};


struct Face {
    std::map<char, Glyph> glyphs;

};

void get_mesh_F(GlyphMesh& mesh);
void get_mesh_A(GlyphMesh& mesh);


}
