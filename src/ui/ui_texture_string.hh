
#ifndef UI_TEXTURE_STRING_HH
#define UI_TEXTURE_STRING_HH

#include <vector>

#include "image/bitmap.hh"


namespace UI {


/** Init function */
void init_font();

/** Return a bitmap containing the passed character [80x150 pixels]. */
pimage::Bitmap get_char_bitmap(char ch);


/** Updates the passed texture id with a new texture of character placed in a SINGLE ROW */
void update_texture_with_string_row(unsigned int& texture_id_to_update, std::string stringToRender);


}


#endif