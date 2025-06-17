
#include <vector>

#include "lib/minmax.hh"

#include "math/vecmat.hh"

#include "glyph_string.hh"

namespace UI
{
namespace Font
{




const std::vector<GlyphFontBitmap>& GlyphString::
get_glyphs()
{
    return glyphs;
}



void GlyphString::
generate_glyphs_whole(Str& str, size_t glyph_count, float glyph_width)
{
    while(glyphs.size() > 0)
        glyphs.pop_back();
    

    for(size_t i = 0; i < glyph_count; i++)
    {
        GlyphFontBitmap& glyph = glyphs.emplace_back();
        
        // Texture coord
        float text_offset = Font::char_to_texture_y_offset(str[i]);
        glyph.set_texture_y(text_offset);

        // Position offset
        float i_f = (float) i;
        float pos_x_offset = i_f * glyph_width;
        glyph.set_x_offset(pos_x_offset);
    }

}




void GlyphString::generate_glyphs_fractional(Str& str, size_t glyph_count_whole, float glyph_width, float width_last_glyph_normalized)
{

    generate_glyphs_whole(str, glyph_count_whole, glyph_width);



    GlyphFontBitmap& last_glyph = glyphs.emplace_back();

    size_t glyph_index = glyph_count_whole;

    // Texture coord for correct char
    float texture_y_offset = Font::char_to_texture_y_offset(str[glyph_index]);
    last_glyph.set_texture_y(texture_y_offset);

    // Sample only texture width proportional to width of last glyph
    last_glyph.set_texture_x_width(width_last_glyph_normalized);


    // Position offset
    float glyph_index_f = (float) glyph_index;
    float pos_x_offset = glyph_index_f * glyph_width;
    last_glyph.set_x_offset(pos_x_offset);

    // Set glyph width to fraction
    last_glyph.set_normalized_width(width_last_glyph_normalized);

}



}
}