

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
