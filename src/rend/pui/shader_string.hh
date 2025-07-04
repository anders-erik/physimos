
#pragma once

#include <vector>

#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/font/font.hh"

#include "ui/ui_shader_string.hh" // VertexTexture


namespace UI {




class ShaderString : public opengl::ShaderProgram
{
    unsigned int vao;
    unsigned int vbo;

    uint    uiViewportTransformLoc;
    uint    uiPrimitiveTransformLoc;
    uint    font_unit_LOC;

    unsigned int texture;

    unsigned int glyph_count;
    const unsigned int verts_per_glyph = 6;

public:

    ShaderString() : ShaderProgram("ui/base_string") {};


    void init();
    void set_viewport_transform(m4f4 _viewport_transform);
    void set_texture(unsigned int new_texture);

    void set_glyph_data(const std::vector<UI::Font::GlyphFontBitmap>& glyphs);

    /** Shader program need glyph size for proper height values i think... */
    void set_base_transform(f2 base_location, f2 glyph_size) const;

    void draw() const;

};





}