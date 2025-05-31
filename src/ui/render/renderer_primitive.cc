
#include "math/vecmat.hh"
#include "opengl/texture.hh"

#include "glad/glad.h"
#include <iostream>
#include <cmath>

#include "renderer_primitive.hh"


namespace UI {


const unsigned int SCREEN_INIT_WIDTH = 1200;
const unsigned int SCREEN_INIT_HEIGHT = 800;



// BOOTSTRAP FOR INITIAL TESTING - 2025-05-29
bool renderer_created = false;
RendererPrimitive* renderer_ui;
RendererPrimitive& get_renderer_ui(){
    if(!renderer_created){
        renderer_ui = new RendererPrimitive();
            renderer_created = true;
    }
    return *renderer_ui;
}


RendererPrimitive::RendererPrimitive(){

    viewport_transform.x.x = 2.0f / (float)SCREEN_INIT_WIDTH;
    viewport_transform.y.y = 2.0f / (float)SCREEN_INIT_HEIGHT;
    viewport_transform.x.w = -1.0f;
    viewport_transform.y.w = -1.0f;


    program_color.init();
    program_color.set_viewport_transform(viewport_transform);

    program_texture_.init();
    program_texture_.set_viewport_transform(viewport_transform);


    // Init others
    program_string_id = opengl::build_program_vert_frag(program_string);
}



void RendererPrimitive::set_window_info(f2 size , f2 scale){

    viewport_transform.x.x = 2.0f * scale.x / size.x;
    viewport_transform.y.y = 2.0f * scale.y / size.y;

    program_color.set_viewport_transform(viewport_transform);
    program_texture_.set_viewport_transform(viewport_transform);

}


void RendererPrimitive::draw_color_primitive(UI::Primitive& primitive){

    program_color.set(
        primitive.uiTransform.M_m_s.pointer(), 
        primitive.darkness_shift, 
        primitive.color
    );

    program_color.draw();

}

void RendererPrimitive::draw_texture_primitive(UI::Primitive & primitive){

    program_texture_.set(
        primitive.uiTransform.M_m_s.pointer(), 
        primitive.renderedTexture
    );

    program_texture_.draw();  

}



}
