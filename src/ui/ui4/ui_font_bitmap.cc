

#include "lib/bitmap.hh"

#include "io/bmp2/BMP.hh"

#include "ui_font_bitmap.hh"



Bitmap font_bitmap = {0,0};

Bitmap& get_font_bitmap()
{
    if(font_bitmap.w() == 0)
    {
        font_bitmap = BMPIO::SImport_PX32("resources/ui/font/characters-2-tall.bmp");
        font_bitmap.set_format(PX32F::ARGB);
    }
    
    return font_bitmap;
}


Bitmap get_char_bitmap(uint8_t _character)
{
    Bitmap& font = get_font_bitmap();

    uint letter_height_offset = (_character - 30) * 150;
    u2 pos = {0, letter_height_offset};
    u2 size = {80, 150};

    Bitmap char_bitmap = font.get_subbitmap(pos, size);
    
    return char_bitmap;
}
