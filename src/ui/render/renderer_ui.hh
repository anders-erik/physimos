
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/font_bitmap.hh"

#include "ui/render/program_base.hh"

#include "ui/render/program_primitive_color.hh"
#include "ui/render/program_primitive_texture.hh"
#include "ui/render/program_primitive_string.hh"


namespace UI {

struct Base;
struct Primitive;


// BOOTSTRAP FOR INITIAL TRANSITION TO RENDERER - 2025-05-29
struct RendererUI;
RendererUI& get_renderer_ui();




class RendererUI {

    // Shared matrix across ui shader programs
    m4f4 viewport_transform;

    ProgramBase program_base;

    ProgramPrimitiveColor program_color;
    ProgramPrimitiveTexture program_texture;

    ProgramPrimitiveString program_string;
    std::vector<VertexFontBitmap> char_verts;
    FontBitmap font_bitmap;


public:

    RendererUI();

    void set_window_info(f2 size, f2 scale);

    void draw(UI::Primitive& primitive);

    void draw_base(UI::Base& base);

private:


    void draw_primitive_color(UI::Primitive& primitive);
    void draw_primitive_texture(UI::Primitive& primitive);
    void draw_primitive_string(UI::Primitive& primitive);

};


}