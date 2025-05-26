
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer_quad_2d.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"
#include "math/geometry/shape.hh"

#include "scene/camera_2d.hh"

#include "phont/phont.hh"

struct GLFWWindow;



namespace scene {




struct CursorContext2D {
    f2 viewport_sane; // [0,window_width]x[0,window_height] [pixels]
    f2 viewport_norm; // [0,1]x[0,1] [unitless]
    f2 scene; // [box.left,box.right]x[box.bottom, box.top] [scene_units]
    void set_cursor_pos(f2 pos_sane, f2 pos_norm, Box2D camera_box);
};




class Scene2D {

    f2 window_size_f;
    
    Camera2D camera;

    CursorContext2D cursor_context;
    bool panable = false; // Scene can be panned, usually with middle mouse button pressed

    opengl::Scene2DRenderer renderer2D;
    std::vector<opengl::ShapeS2D> quads;

    std::vector<opengl::ShapeS2D> points;
    std::vector<opengl::ShapeS2D> lines;
    std::vector<opengl::ShapeS2D> shapes;

public:


    Scene2D(f2 _window_size);

    void set_window_size(f2 size);

    void handle_input(window::InputEvent input_event);
    void update();
    void render_window();


    void add_quad(opengl::ShapeS2D& quad_);
    void add_shape(Shape& shape);

};



}