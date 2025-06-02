#pragma once


#include "Windowing.hpp"
#include "Input.hpp"

#include "math/vecmat.hh"

#include "ui/render/renderer_primitive.hh"

#include "conductor_common.hh"
#include "conductor_viewport.hh"

#include "ui/ui_globals.hh"
#include "ui/ui_texture.hh"

#include "ui/ui_primitive.hh"

#include "ui/elements/string.hh"

namespace UI {


/** Physimos UI. */
class PUI {

public:

    UI::RendererPrimitive renderer_ui;

    UI::Primitive prim_0;
    UI::Primitive prim_texture;

    UI::String string_prim_0;
    // UI::Primitive string_prim_0;

    PUI() 
        // : string_prim_0 {UI::Primitive("FFFF")}
    {

        // UI::font::init_font();
        UI::texture::init_static_color_textures();
        UI::texture::init_static_icon_textures();

        // renderer_ui = UI::RendererPrimitive();
        // renderer_ui.set_window_info({},{});
        
        // prim_0.render
        prim_0.set_w("200x");
        prim_0.set_h("200x");
        prim_0.set_x("<400x");
        prim_0.set_y("_100x");

        prim_texture.set_w("20x");
        prim_texture.set_h("20x");
        prim_texture.set_x("<20x");
        prim_texture.set_y("_20x");
        prim_texture.set_texture(texture::get_icon(Icon::Right));

        string_prim_0.set_string("HOLA");

        // string_prim_0.set_w("200x");
        // string_prim_0.set_h("50x");

        // string_prim_0.str_setFontSize(FontSize::f18);
        // string_prim_0.str_setString("asd");
        // string_prim_0.str_setFontSize(FontSize::f18);
        // string_prim_0.str_setString("asd");
        // string_prim_0.str_setFontSize(FontSize::f24);
        // string_prim_0.set_x("<400x");
        // string_prim_0.set_y("_10x");

        // prim_0.set_color({});
    };

    void set_window_info(f2 size, f2 scale){
        renderer_ui.set_window_info(size, scale);
    }

    void render(){

        prim_0.render(renderer_ui);
        prim_texture.render(renderer_ui);

        // string_prim_0.render(renderer_ui);
        string_prim_0.render_string(renderer_ui);
    }


};


}
