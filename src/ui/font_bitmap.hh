
#pragma once


#include <vector>

#include "image/bitmap.hh"
#include "image/bmp.hh"


namespace UI {



typedef struct VertexFontBitmap {
    float vx;
    float vy;
    float vz;
    float tx;
    float ty;
    float x_offset;
} VertexFontBitmap;


class FontBitmap {

    pimage::Bitmap  bitmap;
    unsigned int    font_texture;
    /** character width in font bitmap */
    size_t char_width = 80;
    /** character height in font bitmap */
    size_t char_height = 150;

public:

    FontBitmap();

    unsigned int get_font_texture();

    /** Create a list of texture vertices to be passed to the string renderer.  */
    void string_to_texture_vertex_list(
        std::vector<VertexFontBitmap>& list_to_populate, 
        std::string stringToRender
    );



public:
    /** LEGACY [slow as it regenerates the whole texture for each string, every frame]
    Updates the passed texture id with a new texture of character placed in a SINGLE ROW */
    void update_texture_with_string_row(unsigned int& texture_id_to_update, std::string stringToRender);
private:
    /** LEGACY [grab sub-bitmap from large bitmap]:
        Return a bitmap containing the passed character [80x150 pixels]. */
    pimage::Bitmap get_char_bitmap(char ch);

};



}

