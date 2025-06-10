#pragma once

#include "lib/str.hh"

#include "ui/base.hh"
#include "ui/render/renderer_base.hh"

namespace UI {


class BaseString : public Base
{
    
public:

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

        float total_width = glyph_size.x * ( (float) str.size());

        box.size = {total_width, glyph_size.y};
    }

    /** Temporary convenient render method. BaseString owner should instead pass object directly to renderer. */
    void render_string(UI::RendererBase& renderer){
        // renderer.draw_base((*(Base*)this));
        renderer.draw_base_string(*this);
    }

};


}