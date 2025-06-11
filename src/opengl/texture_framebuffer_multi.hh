#pragma once

#include "math/vecmat.hh"

#include "image/bitmap.hh"

namespace opengl {


struct TextureFrameBufferMultisample {
    f4 clear_color;
    i2 size;
    int samples;


    // V2
    unsigned int multisampleTexture;
    unsigned int resolvedTexture; // Regular texture to be rendered
    unsigned int multisampleFBO;
    unsigned int resolvedFBO;

    TextureFrameBufferMultisample() = default;
    TextureFrameBufferMultisample(i2 _text_size, int _samples);

    void reload(i2 _text_size, int _samples);

    void init_gl_objects();
    void multisample_fbo_bind();
    void multisample_fbo_unbind();
    void multisample_fbo_clear_red();
    void multisample_fbo_clear();
    void blit(); /** copy multisampled frambuffer data to resolved framebuffer  */

    unsigned int get_resolved_texture();
};



}