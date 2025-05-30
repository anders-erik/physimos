

#include <iostream>

#include "window/auxwin.hh"

#include "scene/camera2D.hh"
#include "scene/scene2D.hh"
#include "scene/shapeS2D.hh"

#include "opengl/texture.hh"
#include "opengl/renderer2D.hh"

#include "math/shape.hh"


window::InputEvent input_event;



int main(){
    f2 win_size_init {800, 600};

    window::Auxwin auxwin;
    auxwin.init(win_size_init);

    scene::Camera2D camera;

    Shape line = Shape::create(shape_t::line);

    scene::Scene2D scene (win_size_init);



    scene::ShapeS2D quad;
    // opengl::ShapeS2DRenderContext quad_render_context;
    opengl::Scene2DRenderer quad_renderer;

    quad.set_dims(800, 600, 100, 100);

    // quad.transform_2d.scale = {1.0f, 1.0f};
    quad.transform_2d.set_pos(0.0f, 0.0f);
	quad.transform_2d.set_scale(1.0f, 1.0f);

    // opengl::Texture texture (100, 100);
    quad_renderer.create_context_quad_t(quad.render_context, quad.verts_6);
    opengl::set_texture_checker_2x2(quad.render_context.texture);


    camera.set_window_size_px({800, 600});
    // camera.set_width(0.2f);
    camera.set_zoom_factor(1.2f);
    // camera.pan({ -400.0f, -300.0f}); // Half of window size


    std::vector<window::InputEvent> input_events = auxwin.new_frame();

    while (auxwin.end_frame()){
        input_events = auxwin.new_frame();

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