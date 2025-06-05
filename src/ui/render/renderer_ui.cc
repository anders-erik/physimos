
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>


#include "ui/ui_globals.hh" // UI::Color
#include "ui/ui_primitive.hh" // UI::Primtiive
#include "ui/ui_font.hh"

#include "ui/base.hh"
#include "ui/texture.hh"

#include "renderer_ui.hh"


namespace UI {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;



// BOOTSTRAP FOR INITIAL TESTING - 2025-05-29
bool renderer_created = false;
RendererUI* renderer_ui;
/** Get extern renderer. First call initalizes; subsequent calls grab pointer. */
RendererUI& get_renderer_ui(){
    if(!renderer_created){
        renderer_ui = new RendererUI();
            renderer_created = true;
    }
    return *renderer_ui;
}


RendererUI::RendererUI()
{

    // opengl::textures_init();

    viewport_transform.x.x = 2.0f / (float)SCREEN_INIT_WIDTH;
    viewport_transform.y.y = 2.0f / (float)SCREEN_INIT_HEIGHT;
    viewport_transform.x.w = -1.0f;
    viewport_transform.y.w = -1.0f;

    program_base.init();
    program_base.set_viewport_transform(viewport_transform);
    
    program_base_texture.init();
    program_base_texture.set_viewport_transform(viewport_transform);

    program_color.init();
    program_color.set_viewport_transform(viewport_transform);

    program_texture.init();
    program_texture.set_viewport_transform(viewport_transform);

    program_string.init();
    program_string.set_viewport_transform(viewport_transform);
    program_string.set_texture(font_bitmap.get_font_texture());

}



void RendererUI::set_window_info(f2 size , f2 scale){

    viewport_transform.x.x = 2.0f * scale.x / size.x;
    viewport_transform.y.y = 2.0f * scale.y / size.y;

    program_base.set_viewport_transform(viewport_transform);
    program_base_texture.set_viewport_transform(viewport_transform);

    program_color.set_viewport_transform(viewport_transform);
    program_texture.set_viewport_transform(viewport_transform);
    program_string.set_viewport_transform(viewport_transform);

}


void RendererUI::draw(UI::Primitive & primitive){

    // This will only be set once during future instancing
    glDisable(GL_DEPTH_TEST);

    // Always draw the primitive base color first
    draw_primitive_color(primitive);

    // Check if string first, as string might have texture!
    if(primitive.str != "")
        draw_primitive_string(primitive);
    else if(primitive.has_texture)
        draw_primitive_texture(primitive);

}

void RendererUI::draw_base(UI::Base& base){

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

void RendererUI::draw_base_texture(UI::BaseTexture& base_texture){

    // glDisable(GL_DEPTH_TEST);

    unsigned int texture_id = opengl::texture_get_id(opengl::Textures::Colors);

    program_base_texture.set(
        base_texture.get_M_m_s().pointer(), 
        texture_id
    );

    program_base_texture.draw();  

}


void RendererUI::draw_primitive_color(UI::Primitive& primitive){

    program_color.set(
        primitive.uiTransform.M_m_s.pointer(), 
        primitive.darkness_shift, 
        primitive.color
    );

    program_color.draw();

}

void RendererUI::draw_primitive_texture(UI::Primitive & primitive){

    program_texture.set(
        primitive.uiTransform.M_m_s.pointer(), 
        primitive.renderedTexture
    );

    program_texture.draw();  

}


// shader::VertexTexture charVert[6] = {
//     {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   30.0f},   // bottom-left
//     {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  30.0f},   // top-right
//     {0.0f, 1.0f, 0.0f, 0.0f, 0.01f,  30.0f},   // top-left
//     {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  30.0f},   // top-right
//     {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   30.0f},   // bottom-left
//     {1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   30.0f},   // bottom-right
// };


void RendererUI::draw_primitive_string(UI::Primitive & primitive){

    program_string.set_primitive_transform(
        primitive.uiTransform.M_m_s.pointer()
    );

    // font::string_to_texture_vertex_list(char_verts, primitive.str);
    font_bitmap.string_to_texture_vertex_list(char_verts, primitive.str);

    program_string.set_vertex_data(char_verts.data(), char_verts.size() * sizeof(VertexFontBitmap));

    program_string.draw();

}



}
