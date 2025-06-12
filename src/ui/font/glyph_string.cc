
#include <vector>

#include "lib/minmax.hh"

#include "math/vecmat.hh"

#include "glyph_string.hh"

namespace UI
{
namespace Font
{


void GlyphString::
set_str(const Str& new_str)
{
    str = new_str;
}

void GlyphString::set_max_width(float max_width)
{
    this->max_width = max_width;
}

void GlyphString::set_glyph_size(f2 glyph_size)
{
    this->glyph_size = glyph_size;
}

const std::vector<GlyphFontBitmap>& GlyphString::get_glyphs()
{
    return glyphs;
}

void GlyphString::
generate_glyphs()
{
    while(glyphs.size() > 0)
        glyphs.pop_back();

    float max_glyphs_ratio = max_width / glyph_size.x;
    size_t max_whole_glyphs = (size_t) max_glyphs_ratio;

    size_t glyph_count = get_glyph_count(glyph_size.x, max_width, str.size());

    

    for(size_t i = 0; i < glyph_count; i++)
    {
        GlyphFontBitmap& glyph = glyphs.emplace_back();
        
        // Texture coord
        float text_offset = Font::char_to_texture_y_offset(str[i]);
        glyph.set_texture_y(text_offset);

        // Position offset
        float i_f = (float) i;
        float pos_x_offset = i_f * glyph_size.x;
        glyph.set_x_offset(pos_x_offset);
    }

    // all glyphs fit
    if(all_chars_fit(glyph_size.x, max_width, str.size()))
    {
        return;
    }

    

    // 'Cut' the last glyph vertically
    float width_whole_glyphs = (float) max_whole_glyphs * glyph_size.x;
    float width_last_glyph = max_width - width_whole_glyphs;
    float width_last_glyph_normalized = width_last_glyph / glyph_size.x;

    GlyphFontBitmap& glyph = glyphs.emplace_back();

    size_t glyph_index = max_whole_glyphs;

    // Texture coord
    float text_offset = Font::char_to_texture_y_offset(str[glyph_index]);
    glyph.set_texture_y(text_offset);
    // Sample only texture width proportional to width of last glyph
    glyph.set_texture_x_width(width_last_glyph_normalized);


    // Position offset
    float glyph_index_f = (float) glyph_index;
    float pos_x_offset = glyph_index_f * glyph_size.x;
    glyph.set_x_offset(pos_x_offset);

    // Set glyph width to fraction of last glyph
    glyph.set_normalized_width(width_last_glyph_normalized);

}



size_t GlyphString::
get_glyph_count(float glyph_width, float max_texture_width, unsigned int str_size)
{
    float max_to_glyph_ratio = max_texture_width / glyph_size.x;
    size_t max_whole_glyphs = (size_t) max_to_glyph_ratio;
    size_t char_count = (size_t) str_size;

    size_t glyph_count = min(max_whole_glyphs, char_count);

    return glyph_count;
}



bool GlyphString::
all_chars_fit(float glyph_width, float max_texture_width, unsigned int str_size)
{
    float max_to_glyph_ratio = max_texture_width / glyph_size.x;
    size_t max_whole_glyphs = (size_t) max_to_glyph_ratio;
    size_t char_count = (size_t) str_size;

    if(char_count <= max_whole_glyphs)
        return true;
    else
        return false;
}



}
}