
#pragma once

#include "opengl/program.hh"
#include "opengl/renderer_quad_2d.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"


struct GLFWWindow;

namespace scene {


/** Captures [-1, 1]x[-1,1]x[-1, -1] by default. */
struct Camera2D {
    Transform2D transform;
    
    float aspect_ratio = 1.0f;

    void set_aspect_ratio(float ar);

    float zoom_current = 1.0f;
    float zoom_multiplier = 1.1f;
    void set_zoom_multiplier(float multiplier);
    void zoom_set(float zoom);
    void zoom_in();
    void zoom_out();

};



class Scene2D {
public:
    i2 window_size;
    void set_window_size(i2 size);

    Camera2D camera;
    bool mouse_pan = false;

    opengl::TextureFrameBuffer texture_framebuffer_F;

    opengl::Quad2D quad;

    opengl::Quad2DRenderer renderer_quad;

    Scene2D();

    void update();
    void render();

    void handle_input(window::InputEvent input_event);
};



}