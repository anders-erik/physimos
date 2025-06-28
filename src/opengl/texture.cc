
#include <random>
#include <iostream>

#include "glad/glad.h"

// UI
#include "io/bmp/bmp.hh"
#include "lib/process.hh"
// END UI

#include "opengl/error.hh"
#include "texture.hh"

namespace opengl {

unsigned int colors_texture_id;
unsigned int grass_texture_id;
unsigned int checker_texture_id;


// UI CONSTANTS

unsigned int font_texture_id;

 // unsigned int pinkTexture;
unsigned char pinkColor[4] = { 255, 0, 100, 255 };

unsigned int redTexture;
unsigned char redColor[4] = { 255, 0, 0, 255 };


// GRAY
unsigned int darkGrayTexture;
unsigned char darkGrayColor[4] = { 100, 100, 100, 255 };

unsigned int grayTexture;
unsigned char grayColor[4] = { 150, 150, 150, 255 };

unsigned int lightGrayTexture;
unsigned char lightGrayColor[4] = { 200, 200, 200, 255 };


// GREEN
unsigned int darkGreenTexture;
unsigned char darkGreenColor[4] = { 0, 100, 0, 255 };

unsigned int greenTexture;
unsigned char greenColor[4] = { 0, 150, 0, 255 };

unsigned int lightGreenTexture = 0;
unsigned char lightGreenColor[4] = { 0, 200, 0, 255 };

// END UI CONSTANTS


Texture::Texture(i2 _size){
    size.x = _size.x;
    size.y = _size.y;

    new_texture(size.x, size.y);
}
Texture::Texture(int width, int height){
    size.x = width;
    size.y = height;

    new_texture(size.x, size.y);
}
Texture::Texture(Bitmap& bitmap){
    new_texture(bitmap);
}


void Texture::new_texture(int width, int height)
{
    size.x = width;
    size.y = height;

    glGenTextures(1, &id_gl);
    glBindTexture(GL_TEXTURE_2D, id_gl);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, size.x, size.y, GL_FALSE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::draw_rect(i2 _pos, i2 _size, ColorUC _color){
    bind();
    int rect_pixels = _size.x * _size.y;
    int rect_bytes = rect_pixels * sizeof(_color);

    unsigned char bytes[rect_bytes] = {0};

    for(int i = 0; i < rect_bytes; i = i + sizeof(_color)){
        bytes[i+0] = _color.R;
        bytes[i+1] = _color.G;
        bytes[i+2] = _color.B;
        bytes[i+3] = _color.A;
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, _pos.x, _pos.y, _size.x, _size.y,  GL_RGBA,  GL_UNSIGNED_BYTE, bytes);
    unbind();
}


f4 Texture::
get_pixel_color(i2 pos)
{
    f4 vec4_color {0.0f, 0.0f, 0.0f, 0.0f};

    unsigned char pixels_uc[4] = {0, 0, 0, 0};
    float pixels_f[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    opengl::error_check();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindTexture(GL_TEXTURE_2D, id_gl);
    glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_FLOAT, vec4_color.pointer());
    // glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    // glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_FLOAT, pixels_f);

    
    opengl::error_check();
    // std::cout << "pixels : " << pixels[0] << " " << pixels[1] << " " << pixels[2] << " " << pixels[3] << " " << std::endl;
    // std::cout << "pixels : " << pixels_f[0] << " " << pixels_f[1] << " " << pixels_f[2] << " " << pixels_f[3] << " " << std::endl;
    
    
    return vec4_color;
}



void Texture::new_texture(Bitmap& bitmap){
    size.x = bitmap.width;
    size.y = bitmap.height;

    BitmapPixel* pixels_data_raw;

    pixels_data_raw =  bitmap.pixels.data();
    glGenTextures(1, &id_gl);
    glBindTexture(GL_TEXTURE_2D, id_gl);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    
    // glActiveTexture(GL_TEXTURE7);
    

    // glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, id_gl);
}
void Texture::unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}





void textures_init(){

    // OLD UI
    ui__init_font_texture();
    ui__init_static_color_textures();
    ui__init_static_icon_textures();


    // COLORS

    unsigned char colors_texture_data[] = {
        255, 255, 255, 255,
        0,   0,   0,   255, 
        
        255, 0,   0,   255, 
        0,   255, 0,   255, 
        0,   0,   255, 255,

        200, 200, 200, 255, 

        150, 100, 100, 255, 
        150, 100, 100, 255, 
        150, 100, 100, 255, 
        150, 100, 100, 255
    };

    glGenTextures(1, &colors_texture_id);
    glBindTexture(GL_TEXTURE_2D, colors_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 10, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors_texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



    unsigned char grass_texture_data[] = {
        10, 200, 10, 255,
        10, 150, 10, 255,
        10, 100, 10, 255,
        10, 150, 10, 255
    };

    glGenTextures(1, &grass_texture_id);
    glBindTexture(GL_TEXTURE_2D, grass_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);




    float checker_bitmap[16] = {    1.0f, 1.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, 1.0f, 1.0f,
    };

    
    glGenTextures(1, &checker_texture_id);
    glBindTexture(GL_TEXTURE_2D, checker_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Not necessary
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, checker_bitmap);
    glGenerateMipmap(GL_TEXTURE_2D);
}

f2 texture_get_random_grass_color(int seed){


    // stdlib rnadom generator 
    srand(seed);
    int rand_int = rand();

    int value_to_switch = (rand_int) % 4;

    // My own stuff
    // float constant = 123942343;
    // // float x = 123942343 / val;
    // int color_count = 20;
    // float step_size = 1.0f / (float) color_count;
    // int mod = rand_int % color_count;
    // float x = step_size * (float) mod;
    // return { x, 0.5};


    
    switch(value_to_switch){
        case 0:
            return {0.125, 0.5};
        case 1:
            return {0.375, 0.5};
        case 2:
            return {0.625, 0.5};
        case 3:
            return {0.875, 0.5};
    
    }

    return {0.0, 0.0};

}

f2 texture_get_color_coordinate(TextureColors text_color){

    f2 tex_coords;

    switch (text_color){

        case TextureColors::Black:
            tex_coords = {0.05f, 0.5f};
            break;

        case TextureColors::White:
            tex_coords = {0.15f, 0.5f};
            break;

        case TextureColors::Red:
            tex_coords = {0.25f, 0.5f};
            break;
        
        case TextureColors::Green:
            tex_coords = {0.35f, 0.5f};
            break;

        case TextureColors::Blue:
            tex_coords = {0.45f, 0.5f};
            break;

        case TextureColors::LightGray:
            tex_coords = {0.55f, 0.5f};
            break;
    }

    return tex_coords;
}

unsigned int texture_get_id(Textures texture) {
    switch (texture){

        case Textures::Colors:
            return colors_texture_id;
            break;
        case Textures::Grass:
            return grass_texture_id;
            break;
        case Textures::Checker2x2:
            return checker_texture_id;
            break;
        default:
            return colors_texture_id;
            break;
    }
}


void set_texture_checker_2x2(unsigned int& texture_id)
{    
    texture_id = checker_texture_id;
}


























/*

        OLD UI BELOW
*/



unsigned int ui__get_font_bitmap_texture_id()
{
    return font_texture_id;
}

void ui__init_font_texture()
{

    Bitmap bitmap = {Bitmap(0,0)};

    BMP::File BMP_loader;

    std::string phys_dir = physimos_root_dir_or_die();
    std::filesystem::path path(phys_dir + "/resources/ui/font/characters-2-tall.bmp");
    
    bitmap = BMP_loader.load(path);

    // Replace black pixels with transparency
    bitmap.replace_color({0,0,0,255}, {0,0,0,0});

    ui__new_texture(font_texture_id);
    ui__update_with_bitmap(font_texture_id, bitmap);

}






unsigned int 
ui__new_from_bitmap(Bitmap& bitmap)
{
    unsigned int newTexture = 0;

    glGenTextures(1, &newTexture);
    glBindTexture(GL_TEXTURE_2D, newTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, newTexture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    return newTexture;
}


void 
ui__update_with_bitmap(unsigned int textureName, Bitmap& bitmap)
{
    // glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);


    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, textureName);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width, bitmap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}



void 
ui__new_texture(unsigned int& texture_to_init) 
{
    // unsigned int new_pink_texture;

    int imageBufferWidth = 1;
    int imageBufferHeight = 1;
    unsigned char colorBuffer[4] = { 255, 0, 100, 255 };

    glGenTextures(1, &texture_to_init);
    glBindTexture(GL_TEXTURE_2D, texture_to_init);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture_to_init);


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

}




unsigned int 
ui__get_static_color_texture(UI_ColorTexture color) 
{
    switch (color) 
    {
        case UI_ColorTexture::Red:   return redTexture;        break;

        case UI_ColorTexture::Gray:   return grayTexture;        break;
        case UI_ColorTexture::LightGray:   return lightGrayTexture;   break;
        case UI_ColorTexture::DarkGray:   return darkGrayTexture;    break;

        case UI_ColorTexture::Green:   return greenTexture;        break;
        case UI_ColorTexture::LightGreen:   return lightGreenTexture;   break;
        case UI_ColorTexture::DarkGreen:   return darkGreenTexture;    break;

        default:
        return 0;
    break;
    }
}



unsigned int upTexture;
unsigned int downTexture;
unsigned int leftTexture;
unsigned int rightTexture;

unsigned int scrollVertTexture;
unsigned int scrollHoriTexture;
unsigned int panTexture;

unsigned int resizeTexture;



unsigned int 
ui__get_icon(UI_Icon icon)
{
    switch (icon) 
    {
        case UI_Icon::Up:              return upTexture;        break;
        case UI_Icon::Down:            return downTexture;   break;
        case UI_Icon::Left:            return leftTexture;    break;
        case UI_Icon::Right:           return rightTexture;    break;

        case UI_Icon::ScrollVert:      return scrollVertTexture;        break;
        case UI_Icon::ScrollHori:      return scrollHoriTexture;   break;

        case UI_Icon::Pan:             return panTexture;        break;

        case UI_Icon::Resize:             return resizeTexture;        break;

        default: return 0; break;
    }

    return 0;
}




void 
ui__init_static_icon_textures()
{

    // Icons are 20x20
    int icon_width = 20;
    int icon_height = 20;

    std::string phys_dir = physimos_root_dir_or_die();

    // unsigned char colorBuffer[16] = {   0, 0, 0, 255,
    //                                     0, 0, 255, 255,
    //                                     255, 255, 255, 255,
    //                                     255, 255, 255, 255
    // };

    // Bitmap* up_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/pan.bmp");
    // Bitmap* up_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/4x4.bmp");
    // std::vector<pimage::Pixel> pixels = up_bitmap->pixels;
    // pimage::Pixel* pixel_data = pixels.data();
    // unsigned char* pixels_data_raw = (unsigned char*) pixel_data;
    // Print pixels
    // for(int i = 0; i < image_width*image_height*4; i++){
    //     unsigned char tmp_char = *(pixels_data_raw+i);
    //     std::cout << i << " " << std::to_string(tmp_char) << std::endl;
    // }


    BMP::File bmp_loader = BMP::File();
    // Pointer to data used during opengl call
    BitmapPixel* pixels_data_raw;

    //  UP
    Bitmap up_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/up.bmp");
    up_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    // Don't need to cast to char, nor invert image as opengl expects first pixel at lower left corner
    pixels_data_raw =  up_bitmap.pixels.data();
    glGenTextures(1, &upTexture);
    glBindTexture(GL_TEXTURE_2D, upTexture);
    // RENDER TEXTURE BY GRABBING THE NEAREST COLOR VALUE. NO BLENDING.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);


    //  DOWN
    Bitmap down_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/down.bmp");
    down_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    pixels_data_raw =  down_bitmap.pixels.data();
    glGenTextures(1, &downTexture);
    glBindTexture(GL_TEXTURE_2D, downTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);

    //  LEFT
    Bitmap left_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/left.bmp");
    left_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    pixels_data_raw =  left_bitmap.pixels.data();
    glGenTextures(1, &leftTexture);
    glBindTexture(GL_TEXTURE_2D, leftTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);


    //  RIGHT
    Bitmap right_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/right.bmp");
    right_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    pixels_data_raw =  right_bitmap.pixels.data();
    glGenTextures(1, &rightTexture);
    glBindTexture(GL_TEXTURE_2D, rightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);




    //  SCROLL VERT
    Bitmap scroll_vert_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/scroll-vert.bmp");
    scroll_vert_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    pixels_data_raw =  scroll_vert_bitmap.pixels.data();
    glGenTextures(1, &scrollVertTexture);
    glBindTexture(GL_TEXTURE_2D, scrollVertTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);


    //  SCROLL HORI
    Bitmap scroll_hori_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/scroll-hori.bmp");
    scroll_hori_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    pixels_data_raw =  scroll_hori_bitmap.pixels.data();
    glGenTextures(1, &scrollHoriTexture);
    glBindTexture(GL_TEXTURE_2D, scrollHoriTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);



    //  PAN
    Bitmap pan_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/pan.bmp");
    pan_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    pixels_data_raw =  pan_bitmap.pixels.data();
    glGenTextures(1, &panTexture);
    glBindTexture(GL_TEXTURE_2D, panTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);

    //  RESIZE
    Bitmap resize_bitmap = bmp_loader.load(phys_dir + "/resources/ui/icons/resize.bmp");
    resize_bitmap.replace_color({0,0,0,255}, {0,0,0,0});
    pixels_data_raw =  resize_bitmap.pixels.data();
    glGenTextures(1, &resizeTexture);
    glBindTexture(GL_TEXTURE_2D, resizeTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_width, icon_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    glGenerateMipmap(GL_TEXTURE_2D);

}



void 
ui__init_static_color_textures() 
{

    int imageBufferWidth = 1;
    int imageBufferHeight = 1;
    unsigned char colorBuffer[4]; // = { 255, 255, 255, 255 };


    //  RED
    glGenTextures(1, &redTexture);
    glBindTexture(GL_TEXTURE_2D, redTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, redTexture);

    colorBuffer[0] = redColor[0];
    colorBuffer[1] = redColor[1];
    colorBuffer[2] = redColor[2];
    colorBuffer[3] = redColor[3];


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);



    //  GRAY
    glGenTextures(1, &grayTexture);
    glBindTexture(GL_TEXTURE_2D, grayTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, grayTexture);

    colorBuffer[0] = grayColor[0];
    colorBuffer[1] = grayColor[1];
    colorBuffer[2] = grayColor[2];
    colorBuffer[3] = grayColor[3];


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);


    //  LIGHT GRAY
    glGenTextures(1, &lightGrayTexture);
    glBindTexture(GL_TEXTURE_2D, lightGrayTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, lightGrayTexture);

    colorBuffer[0] = lightGrayColor[0];
    colorBuffer[1] = lightGrayColor[1];
    colorBuffer[2] = lightGrayColor[2];
    colorBuffer[3] = lightGrayColor[3];


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);



    //  DARK GRAY
    glGenTextures(1, &darkGrayTexture);
    glBindTexture(GL_TEXTURE_2D, darkGrayTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, darkGrayTexture);

    colorBuffer[0] = darkGrayColor[0];
    colorBuffer[1] = darkGrayColor[1];
    colorBuffer[2] = darkGrayColor[2];
    colorBuffer[3] = darkGrayColor[3];


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);





    //  GREEN
    glGenTextures(1, &greenTexture);
    glBindTexture(GL_TEXTURE_2D, greenTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, greenTexture);

    colorBuffer[0] = greenColor[0];
    colorBuffer[1] = greenColor[1];
    colorBuffer[2] = greenColor[2];
    colorBuffer[3] = greenColor[3];


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);


    //  LIGHT GREEN
    glGenTextures(1, &lightGreenTexture);
    glBindTexture(GL_TEXTURE_2D, lightGreenTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, lightGreenTexture);

    colorBuffer[0] = lightGreenColor[0];
    colorBuffer[1] = lightGreenColor[1];
    colorBuffer[2] = lightGreenColor[2];
    colorBuffer[3] = lightGreenColor[3];


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);



    //  DARK GREEN
    glGenTextures(1, &darkGreenTexture);
    glBindTexture(GL_TEXTURE_2D, darkGreenTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, darkGreenTexture);

    colorBuffer[0] = darkGreenColor[0];
    colorBuffer[1] = darkGreenColor[1];
    colorBuffer[2] = darkGreenColor[2];
    colorBuffer[3] = darkGreenColor[3];


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);
}




}