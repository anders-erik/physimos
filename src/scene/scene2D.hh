
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

#include "scene/subscene2D.hh"

#include "phont/phont.hh"


struct GLFWWindow;


namespace scene {

struct SubScene2D;

/** Cursor movement event in normalized viewport coordinated. Usually recieved from owner of scene.
    [0,1]x[0,1] [unitless]
 */
struct PointerMovement2D {
    f2 pos_prev; // Position of the cursor during previous frame
    f2 pos_curr; // Current cursor position
};
struct PointerClick2D {
    f2 pos_scene_normal; // Position of the cursor during previous frame
    window::MouseButtonEvent button_event;
    
    PointerClick2D(f2 pos_scene_normal, window::MouseButtonEvent button_event)
        :   pos_scene_normal    {pos_scene_normal},
            button_event        {button_event} {};
};




struct Scene2D;
struct SubScene2D;

class Scene2D {

    size_t id;
    size_t parent_id = 0;

    f2 window_size_f;
    f2 cursor_pos_scene;
    f2 cursor_pos_normal;
    
    Camera2D camera;

    bool panable = false; // Scene can be panned, usually with middle mouse button pressed

    opengl::Scene2DRenderer renderer2D;

    m3f3 frame_M_m_s; // dummy frame matrix for testing
    // ShapeS2D* current_target;
    

    std::vector<ShapeS2D> points;
    std::vector<ShapeS2D> lines;
    std::vector<ShapeS2D> shapes;

    std::vector<SubScene2D> subscenes;

public:
    // UI INTERFACE
    Str name;
    std::vector<QuadS2D> quads; // Actual quads

    opengl::TextureFrameBufferMultisample framebuffer;

    QuadS2D* quad_current_hover = nullptr;
    QuadS2D* quad_current_selected = nullptr;
    scene::SubScene2D* subscene_current_hover = nullptr;
    scene::SubScene2D* subscene_current_selected = nullptr;
    void clear_hovers();



    Scene2D(f2 _window_size);

    void   set_id(size_t id);
    size_t get_id();
    void   set_parent_id(size_t id);
    size_t get_parent_id();

    f2 get_window_size();
    void set_window_size(f2 size);
    void set_camera_width(float width);

    // Check if position is within the bounding box of a quad
    QuadS2D* try_match_cursor_to_quad(f2 pos_scene);

    void handle_input(window::InputEvent input_event); // OLD

    f2 normalized_to_scene_conversion(f2 normalized);
    /** Try find which scene that captures the cursor */
    Scene2D* try_find_target_scene(f2 normalized);
    void handle_pointer_move(PointerMovement2D cursor_event);
    void handle_pointer_click(PointerClick2D pointer_click);
    void handle_scroll(float delta);

    void update();

    void render_subscene_textures();

    void render_to_window();
    unsigned int render_to_texture();
    void render();

    void add_quad(scene::QuadS2D& quad);
    ShapeS2D& add_shape(Shape& shape);
    /** Add subscene at position and with size. This is dimensions in current scene coord.  */
    SubScene2D& add_subscene(f2 pos_scene, f2 size_scene);
    // SubScene2D* add_subscene(f2 pos_scene, f2 size_scene);
    // void add_subscene(f2 pos_scene, f2 size_scene);

    void print_info();
};



}