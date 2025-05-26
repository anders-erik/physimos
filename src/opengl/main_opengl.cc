

#include <iostream>

#include "window/auxwin.hh"

#include "scene/camera_2d.hh"
#include "scene/scene_2d.hh"

#include "opengl/texture.hh"
#include "opengl/renderer_quad_2d.hh"

#include "lib/json/examples/shape.hh"


window::InputEvent input_event;



int main(){
    f2 win_size_init {800, 600};

    window::Auxwin auxwin;
    auxwin.init(win_size_init);

    scene::Camera2D camera;

    Shape line = Shape::create(shape_t::line);

    scene::Scene2D scene (win_size_init);



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
    // camera.set_width(0.2f);
    camera.set_zoom_factor(1.2f);
    // camera.pan({ -400.0f, -300.0f}); // Half of window size


    std::vector<window::InputEvent> input_events = auxwin.new_frame_2();

    while (auxwin.end_frame_2()){
        input_events = auxwin.new_frame_2();

        for(auto& event : input_events){}

        // camera.transform.set_matrix_camera();
        // quad.transform_2d.set_matrix_model();

        // quad_renderer.activate();
        // quad_renderer.set_camera(camera.transform.matrix);
        
        // // Loop al models here
        // quad_renderer.set_model(quad.transform_2d.matrix);

        // quad_renderer.render(quad.render_context);


    }
    


    return 0;
}