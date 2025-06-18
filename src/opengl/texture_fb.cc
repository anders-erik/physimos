
#include <random>
#include <iostream>

#include "glad/glad.h"


#include "texture_fb.hh"


namespace opengl {



TextureFB::TextureFB(i2 _size)
    // : texture { Texture(_size) }
{
    reload(_size.x, _size.y);
}
TextureFB::TextureFB(int width, int height)
    // : texture { Texture(width, height) }
{
    reload(width, height);
}

void TextureFB::reload(int width, int height)
{
    size.x = width;
    size.y = height;

    texture.new_texture(width, height);

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


void TextureFB::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_MULTISAMPLE);
    glViewport(0,0, size.x, size.y);
}
void TextureFB::unbind(f2 window_fb_size)
{
    // glDisable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, (unsigned int)window_fb_size.x, (unsigned int)window_fb_size.y);
}


void TextureFB::
framebuffer_bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_MULTISAMPLE);
    glViewport(0,0, size.x, size.y);
}

void TextureFB::
framebuffer_unbind(int width, int height)
{
    // glDisable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, width, height);
}




unsigned int TextureFB::
get_texture_id()
{
    return texture.id_gl;
}

f4 TextureFB::sample_texture(i2 pos)
{
    
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    GLubyte pixel[4]; // for RGBA

    f4 vec4_color;

    glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_FLOAT, &vec4_color);

    return vec4_color;
    // return texture.get_pixel_color(pos);
}


void TextureFB::
set_clear_color(f4 color)
{
    clear_color = color;
}


void TextureFB::
clear_with(f4 color)
{
    glClearColor(   color.x, 
                    color.y, 
                    color.z, 
                    color.w
    );
    
    glClear(GL_COLOR_BUFFER_BIT);
}


void TextureFB::
clear()
{
    glClearColor(   clear_color.x, 
                    clear_color.y, 
                    clear_color.z, 
                    clear_color.w
    );
    
    glClear(GL_COLOR_BUFFER_BIT);
}


void TextureFB::
set_size(i2 _size)
{
    size = _size;
}



}