
#pragma once

#include <iostream>

#include "window/auxwin.hh"

#include "opengl/texture.hh"
#include "opengl/renderer_quad_2d.hh"

window::InputEvent input_event;

int main(){

    window::Auxwin auxwin (800, 600);

    opengl::Quad2D quad;
    // opengl::Quad2DRenderContext quad_render_context;
    opengl::Quad2DRenderer quad_renderer;

    quad.set_dims(800, 600, 100, 100);

    // opengl::Texture texture (100, 100);
    quad_renderer.create_context(quad);
    opengl::set_texture_checker_2x2(quad.render_context.texture);


    while (auxwin.is_open())
    {
        auxwin.new_frame();
        
        
        while(input_event = auxwin.get_input_event(), input_event.type != window::EventType::None){

            if(input_event.key_stroke.key == window::Key::Esc)
                auxwin.close();
            
        }

        quad.transform_2d.set_matrix_model();
        quad_renderer.render(quad.render_context);


        auxwin.end_frame();
    }
    


    return 0;
}