#include <string>
#include <filesystem>
#include <vector>

#include "image/bitmap.hh"
#include "image/bmp.hh"

#include "ui/ui_globals.hh"

#include "ui/ui_texture.hh"
#include "ui/ui_texture_string.hh"



namespace UI {


pimage::io::BMP BMP_loader;
pimage::Bitmap  font_bitmap(0,0);
/** character width in font bitmap */
size_t char_width = 80;
/** character height in font bitmap */
size_t char_height = 150;



// Read whole ascii bitmap from file and extract all characters as individual bitmaps
void init_font(){

    std::filesystem::path path("/home/anders/dev/Physimos/resources/ui/font/characters-2-tall.bmp");
    font_bitmap = BMP_loader.load(path);

    // Replace black pixels with transparency
    font_bitmap.replace_color({0,0,0,255}, {0,0,0,0});

}

pimage::Bitmap get_char_bitmap(char ch){
    unsigned int index_offset = ch - 30;
    unsigned int y_pixel_offset = index_offset * char_height;
    return font_bitmap.get_sub_bitmap(0, y_pixel_offset, char_width, char_height);
}


void update_texture_with_string_row(unsigned int& texture_id_to_update, std::string stringToRender) {

    unsigned int string_size = stringToRender.size();
    
    // 
    pimage::Bitmap char_row(string_size * char_width, char_height);

    // Grab each character, get the character bitmap, then
    for(size_t i = 0; i < stringToRender.size(); i++){
        char ch = stringToRender[i];
        pimage::Bitmap char_bitmap = get_char_bitmap(ch);
        char_row.set_sub_bitmap(char_width*i, 0, char_bitmap);

    }
    
    UI::texture::update_with_bitmap(texture_id_to_update, char_row);

}




}