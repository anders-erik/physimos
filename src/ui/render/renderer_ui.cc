
#include <iostream>

#include "glad/glad.h"

#include "opengl/texture.hh"

#include "math/vecmat.hh"

#include "ui/ui_globals.hh" // UI::Color
#include "ui/ui_texture.hh" // old ui texture module

#include "ui/base.hh"
#include "ui/texture.hh"
#include "ui/string.hh"


#include "renderer_ui.hh"


namespace UI {


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
    program_string.set_texture(font_bitmap.get_font_texture());

}



void RendererBase::set_window_info(f2 size , f2 scale){

    viewport_transform.x.x = 2.0f * scale.x / size.x;
    viewport_transform.y.y = 2.0f * scale.y / size.y;

    program_base.set_viewport_transform(viewport_transform);
    program_texture.set_viewport_transform(viewport_transform);
    program_string.set_viewport_transform(viewport_transform);


}



void RendererBase::draw_base(UI::Base& base){

    glDisable(GL_DEPTH_TEST);

    

    float darkness_shift = 0.0f;

    if(base.mouse_is_down()){
        // base.mouse_up();
        darkness_shift = -0.4f;
    }
    else if(base.is_hovered()){
        base.unset_hover();
        darkness_shift = -0.2f;
    }
    

    Color color = active_pallete.base1;


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

void RendererBase::draw_base_string(UI::BaseString & base_string)
{

    font_bitmap.string_to_texture_vertex_list(char_verts, "primitivestr");

    program_string.set_base_transform(
        base_string.get_M_m_s().pointer()
    );

    // program_string.set_texture(font_bitmap.get_font_texture()); // set in renderer constructor

    program_string.set_vertex_data(char_verts.data(), char_verts.size() * sizeof(VertexFontBitmap));

    program_string.draw();
}




}
