
#include <random>
#include <iostream>

#include "glad/glad.h"


#include "texture_framebuffer_multi.hh"

namespace opengl {




TextureFrameBufferMultisample::TextureFrameBufferMultisample(i2 _text_size, int _samples)
{
    reload(_text_size, _samples);
}

void TextureFrameBufferMultisample::reload(i2 _text_size, int _samples)
{
    size = _text_size;
    samples = _samples;

    init_gl_objects();

    multisample_fbo_bind();
    multisample_fbo_clear_red();

    blit();

    // Unbind framebuffers
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureFrameBufferMultisample::init_gl_objects(){


    glGenTextures(1, &multisampleTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, size.x, size.y, GL_TRUE);

    glGenFramebuffers(1, &multisampleFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampleTexture, 0);


    glGenTextures(1, &resolvedTexture);
    glBindTexture(GL_TEXTURE_2D, resolvedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &resolvedFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, resolvedFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resolvedTexture, 0);

}

void TextureFrameBufferMultisample::multisample_fbo_bind(){
    glViewport(0,0, size.x, size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
}
void TextureFrameBufferMultisample::multisample_fbo_unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, 800, 600);
}

void TextureFrameBufferMultisample::multisample_fbo_clear_red(){
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
void TextureFrameBufferMultisample::multisample_fbo_clear(){
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}


void TextureFrameBufferMultisample::blit(){
    glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolvedFBO);
    glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int TextureFrameBufferMultisample::get_resolved_texture(){
    return resolvedTexture;
}





}