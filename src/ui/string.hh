#pragma once

#include "lib/str.hh"

#include "ui/base.hh"
#include "ui/render/renderer_base.hh"

namespace UI {


class BaseString : public Base
{
    
public:

    // will be cut the this width during rendering -- sane coordinates
    float max_width = 103.0f; // OK
    // float max_width = 140.0f; // index out of bounds
    // float max_width = 150.0f; // index out of bounds
    // float max_width = 200.0f; // OK

    // this f2 determines the size of ALL glyphs rendered
    f2 glyph_size = {
        9.0f,
        18.0f
    };

    Str str = "DEFAULT STRING NAME";

    BaseString() {
        set_rgba_color(0x440000ff);
    }

    void set_str(const Str& new_str)
    {
        str = new_str;

        float total_str_width = glyph_size.x * ( (float) str.size());

        float actual_width = total_str_width < max_width ? total_str_width : max_width;

        box.size = {actual_width, glyph_size.y};
    }

    /** Temporary convenient render method. BaseString owner should instead pass object directly to renderer. */
    void render_string(UI::RendererBase& renderer){
        // renderer.draw_base((*(Base*)this));
        renderer.draw_base_string(*this);
    }

};


}