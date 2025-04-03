
#pragma once

#include "opengl/program.hh"
#include "opengl/renderer_quad_2d.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"


struct GLFWWindow;

namespace scene {


struct Box2D {
    f2 pos;
    f2 size;

    void print();
};

/** Captures [-1, 1]x[-1,1]x[-1, -1] by default. */
struct Camera2D {

    /** Camera size and offset in scene coordinates. */
    Box2D box;

    /** Number of scene units per pixel. */
    float units_per_px = 0.0f;

    Transform2D transform;

    void reload_dims();
    
    f2 window_size_px;
    float aspect_ratio = 1.0f;
    void set_window_size_px(f2 size);
    // void set_aspect_ratio(float ar);


    float zoom_current = 1.0f;
    float zoom_multiplier = 1.1f;
    void set_zoom_multiplier(float multiplier);
    void zoom_set(float zoom);
    void zoom_in();
    void zoom_out();

    
    f2 cursor_viewport_px; // [0,window_width]x[0,window_height] [pixels]
    f2 cursor_viewport_norm; // [0,1]x[0,1] [unitless]
    f2 cursor_viewport_scene; // [box.left,box.right]x[box.bottom, box.top] [scene_units]
    void set_cursor_pos(f2 pos_px, f2 pos_norm);

    void pan(f2 delta_px);

private:
    float width_initial = 2.0f;

};



class Scene2D {
public:
    f2 window_size;
    void set_window_size(f2 size);

    Camera2D camera;
    bool mouse_pan = false;

    opengl::TextureFrameBuffer texture_framebuffer_F;

    opengl::Quad2D quad;

    opengl::Quad2DRenderer renderer_quad;

    Scene2D(f2 _window_size);

    void update();
    void render();

    void handle_input(window::InputEvent input_event);
};



}