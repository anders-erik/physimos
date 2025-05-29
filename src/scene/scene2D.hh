
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer2D.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"
#include "math/geometry/shape.hh"

#include "scene/camera2D.hh"
#include "scene/shapeS2D.hh"

#include "phont/phont.hh"

struct GLFWWindow;



namespace scene {


/** Cursor movement event in normalized viewport coordinated. Usually recieved from over of scene.
    [0,1]x[0,1] [unitless]
 */
struct PointerMovement2D {
    f2 pos_prev; // Position of the cursor during previous frame
    f2 pos_curr; // Current cursor position
};




class Scene2D {

    f2 window_size_f;

    f2 window_norm_box_pos;
    f2 window_norm_box_size;
    
    Camera2D camera;

    bool panable = false; // Scene can be panned, usually with middle mouse button pressed

    opengl::Scene2DRenderer renderer2D;
    std::vector<ShapeS2D> quads;

    std::vector<ShapeS2D> points;
    std::vector<ShapeS2D> lines;
    std::vector<ShapeS2D> shapes;


public:
    opengl::TextureFrameBufferMultisample framebuffer;


    Scene2D(f2 _window_size);

    void set_window_size(f2 size);
    void set_window_norm_box(f2 pos_normalized, f2 size_normalized);


    void handle_input(window::InputEvent input_event);
    void handle_pointer_move(PointerMovement2D cursor_event);
    void handle_scroll(float delta);

    void update();

    void render_window();
    unsigned int render_texture();


    void add_quad(ShapeS2D& quad_);
    void add_shape(Shape& shape);

    void print();
};



}