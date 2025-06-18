#include <string>
#include <iostream>
#include <filesystem>
#include <vector>

#include "lib/str.hh"
#include "lib/minmax.hh"

#include "render/bitmap.hh"
#include "file_format/bmp/bmp.hh"

#include "ui/ui_globals.hh"

#include "ui/ui_texture.hh"
#include "lib/process.hh"

#include "ui/font/font_bitmap.hh"
#include "font_bitmap.hh"


namespace UI {




// Upper left triangle
// VertexFontBitmap vert0 = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f},  0.0f};
// VertexFontBitmap vert1 = {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f};
// VertexFontBitmap vert2 = {{0.0f, 1.0f, 0.0f}, {0.0f, 0.01f},  0.0f};
// // lower right triangle
// VertexFontBitmap vert3 = {{1.0f, 1.0f, 0.0f}, {1.0f, 0.01f},  0.0f};
// VertexFontBitmap vert4 = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f },  0.0f};
// VertexFontBitmap vert5 = {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f },  0.0f};






float Font::
char_to_texture_y_offset(char ch)
{
    float bitmap_index = (float) (ch - bitmap_ascii_offset); // which bitmap slot is char at
    
    float text_offset = bitmap_index / 100; // 

    return text_offset;
}





}