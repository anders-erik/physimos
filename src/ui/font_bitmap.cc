#include <string>
#include <iostream>
#include <filesystem>
#include <vector>

#include "lib/str.hh"

#include "image/bitmap.hh"
#include "image/bmp.hh"

#include "ui/ui_globals.hh"

#include "ui/ui_texture.hh"
#include "lib/process.hh"

#include "ui/font_bitmap.hh"
#include "font_bitmap.hh"


namespace UI {




// Upper left triangle
VertexFontBitmap vert0 = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f},  0.0f};
VertexFontBitmap vert1 = {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f};
VertexFontBitmap vert2 = {{0.0f, 1.0f, 0.0f}, {0.0f, 0.01f},  0.0f};
// lower right triangle
VertexFontBitmap vert3 = {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f};
VertexFontBitmap vert4 = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f },  0.0f};
VertexFontBitmap vert5 = {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f },  0.0f};





FontBitmap::FontBitmap()
    :   bitmap {pimage::Bitmap(0,0)}
{
    pimage::io::BMP BMP_loader;

    std::string phys_dir = physimos_root_dir_or_die();
    std::filesystem::path path(phys_dir + "/resources/ui/font/characters-2-tall.bmp");
    
    bitmap = BMP_loader.load(path);

    // Replace black pixels with transparency
    bitmap.replace_color({0,0,0,255}, {0,0,0,0});

    texture::new_texture(font_texture);

    texture::update_with_bitmap(font_texture, bitmap);
}



unsigned int FontBitmap::
get_font_texture()
{
    return font_texture;
}



float FontBitmap::
char_to_texture_y_offset(char ch)
{
    float bitmap_index = (float) (ch - bitmap_ascii_offset); // which bitmap slot is char at
    
    float text_offset = bitmap_index / 100; // 

    return text_offset;
}



void FontBitmap::
str_to_bitmap_glyphs(  std::vector<GlyphFontBitmap>& str_glyphs, 
                                Str& str_to_extract, 
                                float glyph_width,
                                float max_width)
{

    while(str_glyphs.size() > 0)
        str_glyphs.pop_back();


    float max_to_glyph_ratio = max_width / glyph_width;
    size_t max_whole_glyphs = (size_t) max_to_glyph_ratio;

    auto min = [](size_t a, size_t b) -> size_t {
        return a > b ? b : a;
    };

    size_t glyph_count = min(max_whole_glyphs, str_to_extract.size());

    for(size_t i = 0; i < glyph_count; i++)
    {
        GlyphFontBitmap& glyph = str_glyphs.emplace_back(
            vert0,
            vert1,
            vert2,
            vert3,
            vert4,
            vert5
        );
        
        // Texture coord
        float text_offset = char_to_texture_y_offset(str_to_extract[i]);
        glyph.set_texture_y(text_offset);

        // Position offset
        float i_f = (float) i;
        float pos_x_offset = i_f * glyph_width;
        glyph.set_x_offset(pos_x_offset);
    }

    // all glyphs fit
    if(max_whole_glyphs > (float)str_to_extract.size())
    {
        return;
    }

    // 'Cut' the last glyph vertically
    float width_whole_glyphs = (float)max_whole_glyphs * glyph_width;
    float width_last_glyph = max_width - width_whole_glyphs;
    float width_last_glyph_normalized = width_last_glyph / glyph_width;

    GlyphFontBitmap& glyph = str_glyphs.emplace_back(
        vert0,
        vert1,
        vert2,
        vert3,
        vert4,
        vert5
    );

    size_t glyph_index = max_whole_glyphs;

    // Texture coord
    float text_offset = char_to_texture_y_offset(str_to_extract[glyph_index]);
    glyph.set_texture_y(text_offset);
    // Sample only texture width proportional to width of last glyph
    glyph.set_texture_x_width(width_last_glyph_normalized);


    // Position offset
    float glyph_index_f = (float) glyph_index;
    float pos_x_offset = glyph_index_f * glyph_width;
    glyph.set_x_offset(pos_x_offset);

    // Set glyph width to fraction of last glyph
    glyph.set_normalized_width(width_last_glyph_normalized);

}







}