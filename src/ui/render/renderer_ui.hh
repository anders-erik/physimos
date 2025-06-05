
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/font_bitmap.hh"

#include "ui/render/program_base.hh"
#include "ui/render/program_texture.hh"

#include "ui/render/program_primitive_color.hh"
#include "ui/render/program_primitive_texture.hh"
#include "ui/render/program_primitive_string.hh"


namespace UI {

struct Base;
struct BaseTexture;

struct Primitive;


// BOOTSTRAP FOR INITIAL TRANSITION TO RENDERER - 2025-05-29
struct RendererUI;
RendererUI& get_renderer_ui();




class RendererUI {

    // Shared matrix across ui shader programs
    m4f4 viewport_transform;

    ProgramBase program_base;
    ProgramTexture program_texture;


    ProgramPrimitiveColor program_primitive_color;
    ProgramPrimitiveTexture program_primitive_texture;

    ProgramPrimitiveString program_primitive_string;
    std::vector<VertexFontBitmap> char_verts;
    FontBitmap font_bitmap;


public:

    RendererUI();

    void set_window_info(f2 size, f2 scale);

    void draw(UI::Primitive& primitive);

    void draw_base(UI::Base& base);
    void draw_base_texture(UI::BaseTexture& base_texture);

private:


    void draw_primitive_color(UI::Primitive& primitive);
    void draw_primitive_texture(UI::Primitive& primitive);
    void draw_primitive_string(UI::Primitive& primitive);

};


}