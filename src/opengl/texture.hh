#pragma once

#include "math/vecmat.hh"

#include "rend/bitmap.hh"

namespace opengl {

enum class TextureColors : unsigned int {
    Black,
    White,

    Red,
    Green,
    Blue,


    LightGray,
};

typedef enum class UI_Icon {
    Up,
    Down,
    Left,
    Right,

    ScrollVert,
    ScrollHori,

    Pan,

    Resize,

} UI_Icon;


typedef enum class UI_ColorTexture {
    Pink, // uninitialized texture
    Red,  // Common default value

    Gray,
    LightGray,
    DarkGray,

    Green,
    LightGreen,
    DarkGreen,
} UI_ColorTexture;


enum class Textures : unsigned int {
    Colors,
    Grass, 
    Checker2x2,
};

struct ColorUC {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

struct Texture {
    unsigned int id_gl;
    i2 size;

    Texture() = default;
    Texture(int width, int height);
    Texture(i2 _size);
    Texture(Bitmap& bitmap);

    void new_texture(Bitmap& bitmap);
    void new_texture(int width, int height);

    void draw_rect(i2 pos, i2 size, ColorUC color);

    /** Never got to work. Read framebuffer attachent instead. */
    f4 get_pixel_color(i2 pos);

    void bind();
    void unbind();
};



void textures_init();


unsigned int texture_get_id(Textures texture);
f2 texture_get_color_coordinate(TextureColors text_color);
f2 texture_get_random_grass_color(int seed);

/** Legacy : replaced by texture_get_id() */
void set_texture_checker_2x2(unsigned int& texture_id);




// OLD UI METHODS

unsigned int ui__get_font_bitmap_texture_id();
void ui__init_font_texture();

/** Allocates a new texture of size 1x1 with the color pink */
void ui__new_texture(unsigned int& texture_to_init);

unsigned int ui__new_from_bitmap(Bitmap& bitmap);
void ui__update_with_bitmap(unsigned int textureName, Bitmap& bitmap);

/** Loads all ui color textures into vram */
void ui__init_static_color_textures();
/** Loads all ui icon textures from filesystem into vram */
void ui__init_static_icon_textures();

unsigned int ui__get_static_color_texture(UI_ColorTexture color);
unsigned int ui__get_icon(UI_Icon icon);


}