
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/font_bitmap.hh"

#include "ui/render/program_base.hh"
#include "ui/render/program_texture.hh"
#include "ui/render/program_string.hh"



namespace UI {

struct Base;
struct BaseTexture;
struct BaseString;





class RendererBase {

    // Shared matrix across ui shader programs
    m4f4 viewport_transform;

    ProgramBase program_base;
    ProgramTexture program_texture;
    ProgramString program_string;

    // String 
    std::vector<GlyphFontBitmap> glyphs_tmp; // temporarily holds all glyphs for sending to gpu
    FontBitmap font_bitmap;


public:

    RendererBase();

    void set_window_info(f2 size, f2 scale);

    void draw_base(UI::Base& base);
    void draw_base_texture(UI::BaseTexture& base_texture);
    void draw_base_string(UI::BaseString& base_string);

};


}