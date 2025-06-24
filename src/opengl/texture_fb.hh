#pragma once

#include "math/vecmat.hh"

#include "rend/bitmap.hh"

#include "opengl/texture.hh"

namespace opengl {


struct TextureFB {
    opengl::Texture texture;
    
    unsigned int framebuffer;
    f4 clear_color;

    i2 size;

    TextureFB() = default;
    TextureFB(int width, int height);
    TextureFB(i2 _size);

    void reload(int width, int height);

    void bind();
    void unbind(f2 window_fb_size);

    void framebuffer_bind();
    void framebuffer_unbind(int width, int height);

    unsigned int get_texture_id();

    f4 sample_texture(const i2 pos) const;

    void set_clear_color(f4 color);
    void clear_with(f4 clear_color);
    void clear();

    void set_size(i2 size);
};


}