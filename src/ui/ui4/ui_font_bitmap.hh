#pragma once

#include "lib/bitmap.hh"

extern Bitmap& get_font_bitmap_80x15000();
extern Bitmap& get_font_bitmap_12x2300();

extern Bitmap get_char_bitmap(uint8_t _character);
extern Bitmap get_char_bitmap_12x23(uint8_t _character);