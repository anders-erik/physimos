#include <string>
#include <iostream>
#include <filesystem>
#include <vector>

#include "image/bitmap.hh"
#include "image/bmp.hh"

#include "ui/ui_globals.hh"

#include "ui/ui_texture.hh"
#include "ui/ui_font.hh"



namespace UI {

namespace font {

pimage::io::BMP BMP_loader;
pimage::Bitmap  font_bitmap(0,0);
unsigned int    font_texture;
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

    texture::new_texture(font_texture);

    // for(int i = 0; i < 80; i++)
    //     std::cout << "" << font_bitmap.get_pixel(i, 360).R << font_bitmap.get_pixel(i, 360).G << font_bitmap.get_pixel(i, 360).B << font_bitmap.get_pixel(i, 360).A << std::endl;
        
    
    texture::update_with_bitmap(font_texture, font_bitmap);
}

pimage::Bitmap& get_font_bitmap(){
    return font_bitmap;
}

unsigned int get_font_texture(){
    return font_texture;
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

shader::VertexTexture charVertex[6] = {
               {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   0.0f},   // bottom-left
               {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  0.0f},   // top-right
               {0.0f, 1.0f, 0.0f, 0.0f, 0.01f,  0.0f},   // top-left
               {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  0.0f},   // top-right
               {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   0.0f},   // bottom-left
               {1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   0.0f},   // bottom-right
        };

void string_to_texture_vertex_list(std::vector<shader::VertexTexture>& list_to_populate, std::string stringToRender) {

    while(list_to_populate.size() > 0)
        list_to_populate.pop_back();

    unsigned int string_size = stringToRender.size();


    for(size_t i = 0; i < string_size; i++){
        char ch = stringToRender[i];

        unsigned int index_offset = ch - 35;
        float index_offset_fl = (float) index_offset;
        float texture_offset_y = index_offset_fl / 100;

        float render_width = 9.0f;

        shader::VertexTexture vert_0 = charVertex[0];
        vert_0.ty += texture_offset_y;
        vert_0.x_offset = (float) i * render_width;
        shader::VertexTexture vert_1 = charVertex[1];
        vert_1.ty += texture_offset_y;
        vert_1.x_offset = (float) i * render_width;
        shader::VertexTexture vert_2 = charVertex[2];
        vert_2.ty += texture_offset_y;
        vert_2.x_offset = (float) i * render_width;
        shader::VertexTexture vert_3 = charVertex[3];
        vert_3.ty += texture_offset_y;
        vert_3.x_offset = (float) i * render_width;
        shader::VertexTexture vert_4 = charVertex[4];
        vert_4.ty += texture_offset_y;
        vert_4.x_offset = (float) i * render_width;
        shader::VertexTexture vert_5 = charVertex[5];
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


}
}