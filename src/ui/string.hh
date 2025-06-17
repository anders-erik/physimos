#pragma once

#include <cmath>

#include "lib/str.hh"
#include "lib/minmax.hh"

#include "ui/base.hh"
#include "ui/render/renderer_base.hh"

#include "ui/font/glyph_string.hh"

namespace UI {


class BaseString : public Base
{
    
public:

    // will be cut the this width during rendering -- sane coordinates
    // float max_width = 49.5f; // OK
    float max_width = 103.0f; // OK
    // float max_width = 140.0f; // index out of bounds
    // float max_width = 150.0f; // index out of bounds
    // float max_width = 200.0f; // OK

    UI::Font::GlyphString glyph_string;

    float actual_width = -1.0f; // minimum of: 1) sum of all char-widths in str OR, 2) max width

    // this f2 determines the size of ALL glyphs rendered
    f2 glyph_size = {
        9.0f,
        18.0f
    };

    Str str = "DEFAULT STRING NAME";


    BaseString() 
    {
        set_rgba_color(0x440000ff);
    }

    /** Set the string to be rendered. */
    void set_str(const Str& new_str)
    {
        str = new_str;

        // Actual width
        float width_sum_all_chars = glyph_size.x * ( (float) str.size());
        actual_width = min(width_sum_all_chars, max_width);
        box.size = {actual_width, glyph_size.y};

        // Generate glyphs
        if(width_sum_all_chars > max_width)
        {
            float glyph_count_frac = max_width / glyph_size.x;
            float glyph_width_remainder_normalized = glyph_count_frac - floorf(glyph_count_frac);
            size_t glyph_count_whole = (size_t) glyph_count_frac;

            glyph_string.generate_glyphs_fractional(    str, 
                                                        glyph_count_whole, 
                                                        glyph_size.x, 
                                                        glyph_width_remainder_normalized);
        }
        else 
        {
            size_t glyph_count_whole = (size_t) str.size();

            glyph_string.generate_glyphs_whole(         str, 
                                                        glyph_count_whole, 
                                                        glyph_size.x);
        }        
    }

    /** Temporary convenient render method. BaseString owner should instead pass object directly to renderer. */
    void render_string(UI::RendererBase& renderer){
        // renderer.draw_base((*(Base*)this));
        renderer.draw_base_string(*this);
    }

};


}