

#include <cstdlib>

#include "lib/arr.hh"
#include "lib/bitmap.hh"
#include "lib/llist.hh"

#include "math/vecmat.hh"

#include "io/input/user_input.hh"
#include "io/bmp2/BMP.hh"

#include "box.hh"

#include "ui/ui4/ui.hh"

Bitmap font_bitmap = {0,0};


void UI::init_bitmap_assets()
{
    // bitmap_assets.emplace_back(BMPIO::SImport_PX32("resources/ui/font/characters-2-tall.bmp"));
    font_bitmap = BMPIO::SImport_PX32("resources/ui/font/characters-2-tall.bmp");
    // bitmap_assets.push_back(font_bitmap);
    
}

Bitmap& UI::get_bitmap_assets()
{
    return font_bitmap;
    // bitmap_assets.emplace_back(BMPIO::SImport_PX32("resources/ui/font/characters-2-tall.bmp"));
    // font = BMPIO::SImport_PX32("resources/ui/font/characters-2-tall.bmp");
    // bitmap_assets.push_back(font_bitmap);
    
}
