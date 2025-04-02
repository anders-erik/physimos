#pragma once

#include "math/vecmat.hh"

#include "image/bitmap.hh"

namespace opengl {

enum class TextureColors : unsigned int {
    Black,
    White,

    Red,
    Green,
    Blue,


    LightGray,
};


enum class Textures : unsigned int {
    Colors,
    Grass, 
};

struct Texture {
    unsigned int id_gl;
    i2 size;

    Texture(int width, int height);
    Texture(pimage::Bitmap& bitmap);

    void set(pimage::Bitmap& bitmap);

    void bind();
    void unbind();
};


struct TextureFrameBuffer {
    i2 size;
    unsigned int texture_id;
    unsigned int framebuffer;

    f4 clear_color;

    TextureFrameBuffer(int width, int height);

    void framebuffer_bind();
    void framebuffer_unbind(int width, int height);

    void texture_bind();
    void texture_unbind();

    void set_clear_color(f4 color);
    void clear();
};



void textures_init();
unsigned int texture_get_id(Textures texture);
f2 texture_get_color_coordinate(TextureColors text_color);
f2 texture_get_random_grass_color(int seed);


}