
#pragma once


#include "math/vecmat.hh"

#include "opengl/program.hh"

#include "ui/ui_globals.hh" // UI::Color
#include "ui/ui_primitive.hh" // UI::Primtiive

#include "ui/render/program_primitive_color.hh"
#include "ui/render/program_primitive_texture.hh"


namespace UI {


// BOOTSTRAP FOR INITIAL TRANSITION TO RENDERER - 2025-05-29
struct RendererPrimitive;
RendererPrimitive& get_renderer_ui();




class RendererPrimitive {

    // Shared matrix across ui shader programs
    m4f4 viewport_transform;

    ProgramPrimitiveColor program_color;
    ProgramPrimitiveTexture program_texture_;


    opengl::ProgramName program_string = opengl::ProgramName::ui_string;
    unsigned int program_string_id;



public:

    RendererPrimitive();

    void set_window_info(f2 size, f2 scale);


    // COLOR
    void draw_color_primitive(UI::Primitive& primitive);
    void draw_texture_primitive(UI::Primitive& primitive);


};


}