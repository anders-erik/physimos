
#ifndef UI_TEXTURE_STRING_HH
#define UI_TEXTURE_STRING_HH

#include <vector>

#include "image/bitmap.hh"

#include "ui/ui_shader_string.hh"


namespace UI {

namespace font {

/** Init function */
void init_font();

pimage::Bitmap& get_font_bitmap();
unsigned int get_font_texture();

/** Return a bitmap containing the passed character [80x150 pixels]. */
pimage::Bitmap get_char_bitmap(char ch);


/** Updates the passed texture id with a new texture of character placed in a SINGLE ROW */
void update_texture_with_string_row(unsigned int& texture_id_to_update, std::string stringToRender);

/** Create a  list of texture vertices to be passed to the string renderer.  */
void string_to_texture_vertex_list(std::vector<shader::VertexTexture>& list_to_populate, std::string stringToRender);

}
}


#endif