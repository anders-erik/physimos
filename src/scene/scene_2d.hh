
#pragma once

#include "opengl/program.hh"
#include "opengl/renderer_quad_2d.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"

#include "scene/camera_2d.hh"

#include "phont/phont.hh"

struct GLFWWindow;



namespace scene2D {




struct CursorContext2D {
    f2 viewport_px; // [0,window_width]x[0,window_height] [pixels]
    f2 viewport_norm; // [0,1]x[0,1] [unitless]
    f2 scene; // [box.left,box.right]x[box.bottom, box.top] [scene_units]
    void set_cursor_pos(f2 pos_px, f2 pos_norm, Box2D camera_box);
};




class Scene2D {
public:
    f2 window_size;
    void set_window_size(f2 size);

    Camera2D camera;
    bool mouse_pan = false;

    CursorContext2D cursor_context;

	phont::GlyphTextureGenerator F_10_16;

    opengl::Quad2D quad;

    opengl::Quad2DRenderer renderer_quad;

    Scene2D(f2 _window_size);

    void update();
    void render();

    void handle_input(window::InputEvent input_event);
};



}