
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

// #include "ui/font_bitmap.hh"
#include "ui/ui_font.hh"
#include "ui/ui_shader_string.hh"

// #include "ui/render/program_base.hh"
// #include "ui/render/program_texture.hh"
// #include "ui/render/program_string.hh"

#include "ui/render/program_primitive_color.hh"
#include "ui/render/program_primitive_texture.hh"
#include "ui/render/program_primitive_string.hh"


namespace UI {

/** Vertex for a bitmap glyph */
typedef struct VertexFont {
    float vx;
    float vy;
    float vz;
    float tx;
    float ty;
    float x_offset;
} VertexFont;

struct Primitive;


// BOOTSTRAP FOR INITIAL TRANSITION TO RENDERER - 2025-05-29
struct RendererPrimitive;
RendererPrimitive& get_renderer_primitive();




class RendererPrimitive {

    // Shared matrix across ui shader programs
    m4f4 viewport_transform;

    ProgramPrimitiveColor program_primitive_color;
    ProgramPrimitiveTexture program_primitive_texture;

    ProgramPrimitiveString program_primitive_string;
    std::vector<shader::VertexTexture> char_verts;
    // FontBitmap font_bitmap;
    // std::vector<shader::VertexTexture> char_verts;


public:

    RendererPrimitive();

    void set_window_info(f2 size, f2 scale);

    void draw(UI::Primitive& primitive);


private:


    void draw_primitive_color(UI::Primitive& primitive);
    void draw_primitive_texture(UI::Primitive& primitive);
    void draw_primitive_string(UI::Primitive& primitive);

};


}