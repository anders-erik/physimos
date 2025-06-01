#include <string>
#include <iostream>
#include <filesystem>
#include <vector>

#include "image/bitmap.hh"
#include "image/bmp.hh"

#include "ui/ui_globals.hh"

#include "ui/ui_texture.hh"
#include "lib/process.hh"

#include "ui/font_bitmap.hh"
#include "font_bitmap.hh"


namespace UI {


VertexFontBitmap charVertexBitmap[6] = {
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   0.0f},   // bottom-left
    {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  0.0f},   // top-right
    {0.0f, 1.0f, 0.0f, 0.0f, 0.01f,  0.0f},   // top-left
    {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  0.0f},   // top-right
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   0.0f},   // bottom-left
    {1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   0.0f},   // bottom-right
};


FontBitmap::FontBitmap()
    :   bitmap {pimage::Bitmap(0,0)}
{

    pimage::io::BMP BMP_loader;

    std::string phys_dir = physimos_root_dir_or_die();
    std::filesystem::path path(phys_dir + "/resources/ui/font/characters-2-tall.bmp");
    
    bitmap = BMP_loader.load(path);

    // Replace black pixels with transparency
    bitmap.replace_color({0,0,0,255}, {0,0,0,0});

    texture::new_texture(font_texture);

    // for(int i = 0; i < 80; i++)
    //     std::cout << "" << font_bitmap.get_pixel(i, 360).R << font_bitmap.get_pixel(i, 360).G << font_bitmap.get_pixel(i, 360).B << font_bitmap.get_pixel(i, 360).A << std::endl;
        
    
    texture::update_with_bitmap(font_texture, bitmap);

}

unsigned int FontBitmap::get_font_texture(){
    return font_texture;
}




void FontBitmap::string_to_texture_vertex_list(std::vector<VertexFontBitmap>& list_to_populate, std::string stringToRender) {

    while(list_to_populate.size() > 0)
        list_to_populate.pop_back();

    unsigned int string_size = stringToRender.size();


    for(size_t i = 0; i < string_size; i++){
        char ch = stringToRender[i];

        unsigned int index_offset = ch - 35;
        float index_offset_fl = (float) index_offset;
        float texture_offset_y = index_offset_fl / 100;

        float render_width = 9.0f;

        VertexFontBitmap vert_0 = charVertexBitmap[0];
        vert_0.ty += texture_offset_y;
        vert_0.x_offset = (float) i * render_width;
        VertexFontBitmap vert_1 = charVertexBitmap[1];
        vert_1.ty += texture_offset_y;
        vert_1.x_offset = (float) i * render_width;
        VertexFontBitmap vert_2 = charVertexBitmap[2];
        vert_2.ty += texture_offset_y;
        vert_2.x_offset = (float) i * render_width;
        VertexFontBitmap vert_3 = charVertexBitmap[3];
        vert_3.ty += texture_offset_y;
        vert_3.x_offset = (float) i * render_width;
        VertexFontBitmap vert_4 = charVertexBitmap[4];
        vert_4.ty += texture_offset_y;
        vert_4.x_offset = (float) i * render_width;
        VertexFontBitmap vert_5 = charVertexBitmap[5];
        vert_5.ty += texture_offset_y;
        vert_5.x_offset = (float) i * render_width;



        list_to_populate.push_back(vert_0);
        list_to_populate.push_back(vert_1);
        list_to_populate.push_back(vert_2);
        list_to_populate.push_back(vert_3);
        list_to_populate.push_back(vert_4);
        list_to_populate.push_back(vert_5);


        // pimage::Bitmap char_bitmap = get_char_bitmap(ch);
        // char_row.set_sub_bitmap(char_width*i, 0, char_bitmap);

    }

    
    // 
    // pimage::Bitmap char_row(string_size * char_width, char_height);

    // // Grab each character, get the character bitmap, then
    // for(size_t i = 0; i < stringToRender.size(); i++){
    //     char ch = stringToRender[i];
    //     pimage::Bitmap char_bitmap = get_char_bitmap(ch);
    //     char_row.set_sub_bitmap(char_width*i, 0, char_bitmap);

    // }
    
    // UI::texture::update_with_bitmap(texture_id_to_update, char_row);

}











// LEGACY BELOW



pimage::Bitmap FontBitmap::get_char_bitmap(char ch){
    unsigned int index_offset = ch - 30;
    unsigned int y_pixel_offset = index_offset * char_height;
    return bitmap.get_sub_bitmap(0, y_pixel_offset, char_width, char_height);
}


void FontBitmap::update_texture_with_string_row(unsigned int& texture_id_to_update, std::string stringToRender) {

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