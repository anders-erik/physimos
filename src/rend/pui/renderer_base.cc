
#include <iostream>

#include "glad/glad.h"

#include "opengl/texture.hh"

#include "str.hh"

#include "math/vecmat.hh"

#include "ui/ui_texture.hh" // old ui texture module

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"

#include "ui/font/font.hh"

#include "renderer_base.hh"


namespace UI 
{


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;





RendererBase::RendererBase()
{

    opengl::textures_init();
    UI::texture::init_static_color_textures();
    UI::texture::init_static_icon_textures();

    viewport_transform.x.x = 2.0f / (float)SCREEN_INIT_WIDTH;
    viewport_transform.y.y = 2.0f / (float)SCREEN_INIT_HEIGHT;
    viewport_transform.x.w = -1.0f;
    viewport_transform.y.w = -1.0f;

    program_base.init();
    program_base.set_viewport_transform(viewport_transform);
    
    program_texture.init();
    program_texture.set_viewport_transform(viewport_transform);

    program_string.init();
    program_string.set_viewport_transform(viewport_transform);
    // program_string.set_texture(font_bitmap.get_font_texture());
    // program_string.set_texture(font_texture.get_texture_id());

}



void RendererBase::set_window_info(f2 size , f2 scale){

    viewport_transform.x.x = 2.0f * scale.x / size.x;
    viewport_transform.y.y = 2.0f * scale.y / size.y;

    program_base.set_viewport_transform(viewport_transform);
    program_texture.set_viewport_transform(viewport_transform);
    program_string.set_viewport_transform(viewport_transform);


}



void RendererBase::
draw_base(UI::Base& base)
{
    glDisable(GL_DEPTH_TEST);   

    float darkness_shift = 0.0f;

    if(base.is_clicked()){
        // base.mouse_up();
        darkness_shift = -0.4f;
    }
    else if(base.is_hovered()){
        base.unset_hover();
        darkness_shift = -0.2f;
    }

    f4 color = base.get_f4_color();

    program_base.set(
        base.get_M_m_s().pointer(), 
        darkness_shift, 
        color
    );

    program_base.draw();
}


void RendererBase::draw_base_texture(UI::BaseTexture& base_texture){

    // glDisable(GL_DEPTH_TEST);

    unsigned int texture_id = opengl::texture_get_id(opengl::Textures::Colors);

    program_texture.set(
        base_texture.get_M_m_s().pointer(), 
        texture_id
    );

    program_texture.draw();  

}

void RendererBase::draw_base_string(UI::BaseString& base_string)
{
    // Draw the base background color
    draw_base((Base&)base_string);
    
    program_string.set_texture(opengl::ui__get_font_bitmap_texture_id());
    program_string.set_base_transform(base_string.get_box().pos, base_string.glyph_size);
    
    program_string.set_glyph_data(base_string.glyph_string.get_glyphs());

    program_string.draw();
}




}
