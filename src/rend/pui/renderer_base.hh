
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"


#include "ui/font/font.hh"

#include "rend/pui/shader_base.hh"
#include "rend/pui/shader_texture.hh"
#include "rend/pui/shader_string.hh"



namespace UI {

struct Base;
struct BaseTexture;
struct BaseString;





class RendererBase {

    // Shared matrix across ui shader programs
    m4f4 viewport_transform;

    ShaderBase program_base;
    ShaderTexture program_texture;
    ShaderString program_string;

    // String 
    std::vector<UI::Font::GlyphFontBitmap> glyphs_tmp; // temporarily holds all glyphs for sending to gpu


public:

    RendererBase();

    void set_window_info(f2 size, f2 scale);

    void draw_base(UI::Base& base);
    void draw_base_texture(UI::BaseTexture& base_texture);
    void draw_base_string(UI::BaseString& base_string);

};


}