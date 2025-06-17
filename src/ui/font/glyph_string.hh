
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

public:

    const std::vector<GlyphFontBitmap>& get_glyphs();

    /** Generates a whole number of glyphs. */
    void generate_glyphs_whole(Str& str, size_t glyph_count, float glyph_width);

    /** Generates a whole number of glyphs, and then appends a final glyph that is vertically cut. */
    void generate_glyphs_fractional(Str& str, size_t glyph_count_whole, float glyph_width, float width_last_glyph_normalized);

};



}
}