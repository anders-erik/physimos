
#include "math/vecmat.hh"
#include "ui/font/glyph.hh"


namespace UI
{
namespace Font
{


/** Set which char we grab from texture */
void GlyphFontBitmap::set_texture_y(float tex_coord_offset)
{
    v0.tex.y += tex_coord_offset;
    v1.tex.y += tex_coord_offset;
    v2.tex.y += tex_coord_offset;
    v3.tex.y += tex_coord_offset;
    v4.tex.y += tex_coord_offset;
    v5.tex.y += tex_coord_offset;
}

/** Set the glyph texture sampling width as a fraction of the normal width (which is 1). */
void GlyphFontBitmap::set_texture_x_width(float sampled_width)
{
    v1.tex.x *= sampled_width;
    v3.tex.x *= sampled_width;
    v5.tex.x *= sampled_width;
}

/** Set the glyph width as a fraction of the default vertex width */
void GlyphFontBitmap::set_normalized_width(float normalized_width)
{
    v1.pos.x *= normalized_width;
    v3.pos.x *= normalized_width;
    v5.pos.x *= normalized_width;
}

/** Shift the vertex horizontally based on index in string. */
void GlyphFontBitmap::set_x_offset(float pos_x_offset)
{
    v0.x_offset += pos_x_offset;
    v1.x_offset += pos_x_offset;
    v2.x_offset += pos_x_offset;
    v3.x_offset += pos_x_offset;
    v4.x_offset += pos_x_offset;
    v5.x_offset += pos_x_offset;
}



}
}