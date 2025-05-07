

#include <iostream>

#include "window/auxwin.hh"

#include "scene/camera_2d.hh"

#include "opengl/texture.hh"
#include "opengl/renderer_quad_2d.hh"

window::InputEvent input_event;

int main(){

    window::Auxwin auxwin;
    auxwin.init(800, 600);
    
    scene2D::Camera2D camera;

    opengl::Quad2D quad;
    // opengl::Quad2DRenderContext quad_render_context;
    opengl::Quad2DRenderer quad_renderer;

    quad.set_dims(800, 600, 100, 100);

    // quad.transform_2d.scale = {1.0f, 1.0f};
    quad.transform_2d.set_pos(0.0f, 0.0f);
	quad.transform_2d.set_scale(1.0f, 1.0f);

    // opengl::Texture texture (100, 100);
    quad_renderer.create_context(quad);
    opengl::set_texture_checker_2x2(quad.render_context.texture);


    camera.set_window_size_px({800, 600});
    // camera.zoom_set(0.2f);
    camera.set_zoom_multiplier(1.2f);
    // camera.pan({ -400.0f, -300.0f}); // Half of window size


    while (auxwin.is_open())
    {
        auxwin.new_frame();
        
        
        while(input_event = auxwin.get_input_event(), input_event.type != window::EventType::None){

            if(input_event.key_stroke.key == window::Key::Esc)
                auxwin.close();
            
        }

        camera.transform.set_matrix_camera();
        quad.transform_2d.set_matrix_model();

        quad_renderer.set_model_camera(quad.transform_2d.matrix, camera.transform.matrix);
        quad_renderer.render(quad.render_context);


        auxwin.end_frame();
    }
    


    return 0;
}