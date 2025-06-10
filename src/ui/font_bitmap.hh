
#pragma once

#include <vector>

#include "math/vecmat.hh"

#include "image/bitmap.hh"
#include "image/bmp.hh"

struct Str;

namespace UI {


/** Vertex for a bitmap glyph */
typedef struct VertexFontBitmap {
    // float vx;
    // float vy;
    // float vz;
    // float tx;
    // float ty;
    f3 pos;
    f2 tex;
    float x_offset;
} VertexFontBitmap;

/** Vertex collection for a single bitmap glyph */
struct GlyphFontBitmap {
    VertexFontBitmap v0;
    VertexFontBitmap v1;
    VertexFontBitmap v2;
    VertexFontBitmap v3;
    VertexFontBitmap v4;
    VertexFontBitmap v5;

    void set_texture_y(float tex_coord_offset)
    {
        v0.tex.y += tex_coord_offset;
        v1.tex.y += tex_coord_offset;
        v2.tex.y += tex_coord_offset;
        v3.tex.y += tex_coord_offset;
        v4.tex.y += tex_coord_offset;
        v5.tex.y += tex_coord_offset;
    }

    void set_x_offset(float pos_x_offset)
    {
        v0.x_offset += pos_x_offset;
        v1.x_offset += pos_x_offset;
        v2.x_offset += pos_x_offset;
        v3.x_offset += pos_x_offset;
        v4.x_offset += pos_x_offset;
        v5.x_offset += pos_x_offset;
    }
};


class FontBitmap {

    pimage::Bitmap  bitmap;
    unsigned int    font_texture;
    /** character width in font bitmap */
    size_t char_width_px = 80;
    /** character height in font bitmap */
    size_t char_height_px = 150;

    /** The vertical bitmap begins at index 30 */
    int bitmap_ascii_offset = 30;
    /** The number of chars stacked in bitmap texture. */
    float bitmap_char_count = 100;

public:

    FontBitmap();

    unsigned int get_font_texture();

    /** Converts ascii value to the correct y-offset in vertical bitmap texture. 
    Texture coordinates assumed to be [0,1]x[0,1] */
    float char_to_texture_y_offset(char ch);

    /** Create a list of texture vertices to be passed to the string renderer.  */
    void str_to_bitmap_glyphs(
        std::vector<GlyphFontBitmap>& str_glyphs, 
        Str& str_to_extract,
        float glyph_width
    );


};



}

