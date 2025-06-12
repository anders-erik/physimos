
#pragma once

#include <vector>

#include "lib/str.hh"

#include "math/vecmat.hh"

#include "font_bitmap.hh"
#include "glyph.hh"

namespace UI
{
namespace Font
{


/** Collection of Glyphs, each is 6 position and texture coordinates. */
class GlyphString 
{
    std::vector<GlyphFontBitmap> glyphs;
    
    Str str;

    f2 glyph_size;
    float max_width;

public:

    void set_str(const Str& new_str);
    void set_max_width(float max_width);
    void set_glyph_size(f2 glyph_size);

    const std::vector<GlyphFontBitmap>& get_glyphs();

    /** Create a list of texture vertices to be passed to the string renderer.  */
    void generate_glyphs();


    size_t get_glyph_count(float glyph_width, float max_texture_width, unsigned int str_size);
    bool all_chars_fit(float glyph_width, float max_texture_width, unsigned int str_size);

};



}
}