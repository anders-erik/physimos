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

struct ColorUC {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

struct Texture {
    unsigned int id_gl;
    i2 size;

    Texture(int width, int height);
    Texture(i2 _size);
    Texture(pimage::Bitmap& bitmap);

    void set(pimage::Bitmap& bitmap);

    void new_texture(int width, int height);

    void draw_rect(i2 pos, i2 size, ColorUC color);

    static unsigned char* get_pixel(i2 pos, unsigned int texture_id);

    void bind();
    void unbind();
};

struct TextureFrameBufferMultisample {
    f4 clear_color;
    i2 size;
    int samples;


    // V2
    unsigned int multisampleTexture;
    unsigned int resolvedTexture; // Regular texture to be rendered
    unsigned int multisampleFBO;
    unsigned int resolvedFBO;

    TextureFrameBufferMultisample(i2 _text_size, int _samples);

    void init_gl_objects();
    void multisample_fbo_bind();
    void multisample_fbo_unbind();
    void multisample_fbo_clear_red();
    void multisample_fbo_clear();
    void blit(); /** copy multisampled frambuffer data to resolved framebuffer  */

    unsigned int get_resolved_texture();
};

struct TextureFrameBuffer {
    opengl::Texture texture;
    
    unsigned int framebuffer;
    f4 clear_color;

    i2 size;

    TextureFrameBuffer(int width, int height);
    TextureFrameBuffer(i2 _size);

    void init();

    void framebuffer_bind();
    void framebuffer_unbind(int width, int height);

    void texture_bind();
    void texture_unbind();

    void set_clear_color(f4 color);
    void clear();

    void set_size(i2 size);
};



void textures_init();
unsigned int texture_get_id(Textures texture);
f2 texture_get_color_coordinate(TextureColors text_color);
f2 texture_get_random_grass_color(int seed);


void set_texture_checker_2x2(unsigned int& texture_id);


}