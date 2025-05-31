
#pragma once

#include <vector>

#include "opengl/program.hh"
#include "opengl/renderer2D.hh"
#include "opengl/texture.hh"

#include "window/auxwin.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"

#include "scene/camera2D.hh"
#include "scene/shapeS2D.hh"
#include "scene/quadS2D.hh"

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
struct PointerClick2D {
    f2 pos_scene_normal; // Position of the cursor during previous frame
    window::MouseButtonEvent button_event;
};


/** Represents a wireframe used to highlight scene objects. */
class BoxFrame2D : Box2D {

public:
    m3f3 M_m_s;
    std::array<opengl::VertexQuad2D, 8> verts; // Vertices for wireframe highlighting
    opengl::RenderContextQuadS2D render_context;  // Rendering context for wireframe highlighting

};



class Scene2D {

    f2 window_size_f;
    
    Camera2D camera;

    bool panable = false; // Scene can be panned, usually with middle mouse button pressed

    opengl::Scene2DRenderer renderer2D;

    /** Represents a wireframe used to highlight scene objects. */
    BoxFrame2D frame;
    ShapeS2D* current_target;
    
    std::vector<QuadS2D> quads; // Actual quads

    std::vector<ShapeS2D> points;
    std::vector<ShapeS2D> lines;
    std::vector<ShapeS2D> shapes;


public:
    opengl::TextureFrameBufferMultisample framebuffer;


    Scene2D(f2 _window_size);

    f2 get_window_size();
    void set_window_size(f2 size);

    void handle_input(window::InputEvent input_event);
    void handle_pointer_move(PointerMovement2D cursor_event);
    void handle_pointer_click(PointerClick2D pointer_click);
    void handle_scroll(float delta);

    void update();

    void render_window();
    unsigned int render_to_texture();

    // Create the initial frame rendering conetxt
    void init_frame();

    void add_quad(scene::QuadS2D& quad);
    ShapeS2D& add_shape(Shape& shape);

    void print();
};



}