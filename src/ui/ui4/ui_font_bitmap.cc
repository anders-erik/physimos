

#include "lib/bitmap.hh"

#include "io/bmp2/BMP.hh"

#include "ui_font_bitmap.hh"



Bitmap font_bitmap_80x15000 = {0,0};
Bitmap font_bitmap_12x2300  = {0,0};

Bitmap& get_font_bitmap_80x15000()
{
    if(font_bitmap_80x15000.w() == 0)
    {
        font_bitmap_80x15000 = BMPIO::SImport_PX32("resources/ui/font/characters-2-tall.bmp");
        font_bitmap_80x15000.set_format(PX32F::ARGB);
    }
    
    return font_bitmap_80x15000;
}

Bitmap& get_font_bitmap_12x2300()
{
    if(font_bitmap_12x2300.w() == 0)
    {
        font_bitmap_12x2300 = BMPIO::SImport_PX32("resources/ui/font/font-v2-12x2300.bmp");
        font_bitmap_12x2300.set_format(PX32F::ARGB);
    }
    
    return font_bitmap_12x2300;
}


Bitmap get_char_bitmap(uint8_t _character)
{
    Bitmap& font = get_font_bitmap_80x15000();

    uint letter_height_offset = (_character - 30) * 150;
    u2 pos = {0, letter_height_offset};
    u2 size = {80, 150};

    Bitmap char_bitmap = font.get_subbitmap(pos, size);
    
    return char_bitmap;
}

Bitmap get_char_bitmap_12x23(uint8_t _character)
{

    Bitmap& font = get_font_bitmap_12x2300();

    uint char_width = 12;
    uint char_height = 23;

    uint letter_height_offset = (_character - 30) * char_height;
    u2 pos = {0, letter_height_offset};
    u2 size = {char_width, char_height};

    Bitmap char_bitmap = font.get_subbitmap(pos, size);
    
    return char_bitmap;
}