
#pragma once

#include "math/vecmat.hh"

namespace UI
{
namespace Font
{


/** Vertex for a bitmap glyph */
typedef struct VertexFontBitmap 
{
    f3 pos;
    f2 tex;
    float x_offset;

    VertexFontBitmap() = default;

    constexpr VertexFontBitmap(f3 pos, f2 tex, float x_offset) :
        pos {pos},
        tex {tex},
        x_offset {x_offset}
    {
    }

} VertexFontBitmap;



/** Glyph vertices */
constexpr VertexFontBitmap default_glyph[6] = {
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f},  0.0f},
    {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f},
    {{0.0f, 1.0f, 0.0f}, {0.0f, 0.01f},  0.0f},
    {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f },  0.0f},
    {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f },  0.0f}
};



/** Vertex collection for a single bitmap glyph */
struct GlyphFontBitmap 
{
    VertexFontBitmap v0, v1, v2, v3, v4, v5;
    // VertexFontBitmap v0 = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f},  0.0f};
    // VertexFontBitmap v1 = {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f};
    // VertexFontBitmap v2 = {{0.0f, 1.0f, 0.0f}, {0.0f, 0.01f},  0.0f};
    // VertexFontBitmap v3 = {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f};
    // VertexFontBitmap v4 = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f },  0.0f};
    // VertexFontBitmap v5 = {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f },  0.0f};

    constexpr GlyphFontBitmap() 
        :   v0 {default_glyph[0]},
            v1 {default_glyph[1]},
            v2 {default_glyph[2]},
            v3 {default_glyph[3]},
            v4 {default_glyph[4]},
            v5 {default_glyph[5]}
    {
    };


    /** Set which char we grab from texture */
    void set_texture_y(float tex_coord_offset);

    /** Set the glyph texture sampling width as a fraction of the normal width (which is 1). */
    void set_texture_x_width(float sampled_width);

    /** Set the glyph width as a fraction of the default vertex width */
    void set_normalized_width(float normalized_width);

    /** Shift the vertex horizontally based on index in string. */
    void set_x_offset(float pos_x_offset);

};



}
}
