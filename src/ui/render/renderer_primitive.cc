
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>


#include "ui/ui_globals.hh" // UI::Color
#include "ui/ui_primitive.hh" // UI::Primtiive
#include "ui/ui_font.hh"


#include "renderer_primitive.hh"


namespace UI {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;



// BOOTSTRAP FOR INITIAL TESTING - 2025-05-29
static bool renderer_created = false;
RendererPrimitive* renderer_primitive;
/** Get extern renderer. First call initalizes; subsequent calls grab pointer. */
RendererPrimitive& get_renderer_primitive(){
    if(!renderer_created){
        renderer_primitive = new RendererPrimitive();
            renderer_created = true;
    }
    return *renderer_primitive;
}


RendererPrimitive::RendererPrimitive()
{

    // opengl::textures_init();

    viewport_transform.x.x = 2.0f / (float)SCREEN_INIT_WIDTH;
    viewport_transform.y.y = 2.0f / (float)SCREEN_INIT_HEIGHT;
    viewport_transform.x.w = -1.0f;
    viewport_transform.y.w = -1.0f;

    program_primitive_color.init();
    program_primitive_color.set_viewport_transform(viewport_transform);

    program_primitive_texture.init();
    program_primitive_texture.set_viewport_transform(viewport_transform);

    program_primitive_string.init();
    program_primitive_string.set_viewport_transform(viewport_transform);
    program_primitive_string.set_texture(font_bitmap.get_font_texture());

}



void RendererPrimitive::set_window_info(f2 size , f2 scale){

    viewport_transform.x.x = 2.0f * scale.x / size.x;
    viewport_transform.y.y = 2.0f * scale.y / size.y;

    program_primitive_color.set_viewport_transform(viewport_transform);
    program_primitive_texture.set_viewport_transform(viewport_transform);
    program_primitive_string.set_viewport_transform(viewport_transform);

}


void RendererPrimitive::draw(UI::Primitive & primitive){

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


void RendererPrimitive::draw_primitive_color(UI::Primitive& primitive){

    program_primitive_color.set(
        primitive.uiTransform.M_m_s.pointer(), 
        primitive.darkness_shift, 
        primitive.color
    );

    program_primitive_color.draw();

}

void RendererPrimitive::draw_primitive_texture(UI::Primitive & primitive){

    program_primitive_texture.set(
        primitive.uiTransform.M_m_s.pointer(), 
        primitive.renderedTexture
    );

    program_primitive_texture.draw();  

}


// shader::VertexTexture charVert[6] = {
//     {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   30.0f},   // bottom-left
//     {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  30.0f},   // top-right
//     {0.0f, 1.0f, 0.0f, 0.0f, 0.01f,  30.0f},   // top-left
//     {1.0f, 1.0f, 0.0f, 1.0f, 0.01f,  30.0f},   // top-right
//     {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   30.0f},   // bottom-left
//     {1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   30.0f},   // bottom-right
// };


void RendererPrimitive::draw_primitive_string(UI::Primitive & primitive){

    program_primitive_string.set_primitive_transform(
        primitive.uiTransform.M_m_s.pointer()
    );

    Str str = Str(primitive.str.c_str());
    // font::string_to_texture_vertex_list(char_verts, primitive.str);
    font_bitmap.string_to_texture_vertex_list(char_verts, str);

    program_primitive_string.set_vertex_data(char_verts.data(), char_verts.size() * sizeof(VertexFontBitmap));

    program_primitive_string.draw();

}



}
