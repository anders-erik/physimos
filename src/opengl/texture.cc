
#include <random>
#include <iostream>

#include "glad/glad.h"


#include "texture.hh"

namespace opengl {

unsigned int colors_texture_id;
unsigned int grass_texture_id;

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
Texture::Texture(pimage::Bitmap& bitmap){
    set(bitmap);
}


void Texture::new_texture(int width, int height){

    glGenTextures(1, &id_gl);
    glBindTexture(GL_TEXTURE_2D, id_gl);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // glGenerateMipmap(GL_TEXTURE_2D);
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

void Texture::set(pimage::Bitmap& bitmap){
    size.x = bitmap.width;
    size.y = bitmap.height;

    pimage::Pixel* pixels_data_raw;

    pixels_data_raw =  bitmap.pixels.data();
    glGenTextures(1, &id_gl);
    glBindTexture(GL_TEXTURE_2D, id_gl);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data_raw);
    // glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, id_gl);
}
void Texture::unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureFrameBuffer::TextureFrameBuffer(i2 _size)
    : texture { Texture(_size) }
{
    size.x = _size.x;
    size.y = _size.y;
    init();
}
TextureFrameBuffer::TextureFrameBuffer(int width, int height)
    : texture { Texture(width, height) }
{
    size.x = width;
    size.y = height;

    init();

}

void TextureFrameBuffer::init(){


    // FRAMEBUFFER
    // unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id_gl, 0);

    // ERROR
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    else 
        std::cout << "FRAMEBUFFER OK!" << std::endl;

}

void TextureFrameBuffer::framebuffer_bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0,0, size.x, size.y);
}

void TextureFrameBuffer::framebuffer_unbind(int width, int height){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, width, height);
}
void TextureFrameBuffer::texture_bind(){
    glBindTexture(GL_TEXTURE_2D, texture.id_gl);
}
void TextureFrameBuffer::texture_unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}
void TextureFrameBuffer::set_clear_color(f4 color){
    clear_color = color;
}
void TextureFrameBuffer::clear(){
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}
void TextureFrameBuffer::set_size(i2 _size){
    size = _size;
    
}


void textures_init(){

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
        default:
            return colors_texture_id;
            break;
    }
}




void set_texture_checker_2x2(unsigned int& texture_id){

    // unsigned char bitmap[16] = {255, 255, 255, 255,
    //                             0,   0,   0,   255,
    //                             0,   0,   0,   255,
    //                             255, 255, 255, 255,
    // };

    float bitmap[16] = {    1.0f, 1.0f, 1.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, 1.0f, 1.0f,
    };

    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Not necessary
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, bitmap);
    glGenerateMipmap(GL_TEXTURE_2D);


}


}