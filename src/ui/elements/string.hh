#pragma once

#include "ui/ui_primitive.hh"

namespace UI {


struct String : public Primitive {

    Primitive string_prim;

    String() {

    }
    // String() : string_prim {Primitive("STRSTR")} {}

    void set_string(std::string new_string){

        string_prim.str_setFontSize(FontSize::f18);
        string_prim.str_setString(new_string);
        // string_prim.str_setFontSize(FontSize::f18);

        string_prim.set_x("<200x");
        string_prim.set_y("_20x");
        // string_prim.set_w("200x");
        // string_prim.set_h("100x");
        string_prim.set_color( {0.0f, 0.0f, 0.0f, 0.5f} );


    }

    void render_string(UI::RendererUI& renderer){
        render(renderer);
        string_prim.render(renderer);
    }

};


}