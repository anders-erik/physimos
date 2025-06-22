
#pragma once

#include <vector>

#include "lib/pair.hh"

#include "window/auxevent.hh"

#include "opengl/program.hh"
#include "opengl/texture.hh"
#include "opengl/texture_fb.hh"
#include "opengl/texture_fbms.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"
#include "math/graph2D.hh"

#include "rend/scene2D/renderer2D.hh"
#include "scene2D/shapeS2D.hh"
#include "scene2D/camera2D.hh"
#include "scene2D/quad_manager.hh"

#include "sid.hh"

struct GLFWWindow;




struct QuadS2D;


/** Returned when querying for a quad match in a scene. 
quad_id == 0 -> no match */
struct QuadQuery 
{
    size_t quad_id;
    Box2D normalized_viewbox_coords;
};


/** Cursor movement event in normalized viewport coordinates: [0,1]x[0,1] [unitless] */
struct CursorMovementNormalized 
{
    f2 prev; // Position of the cursor during previous frame
    f2 curr; // Current cursor position
};


struct PointerClick2D 
{
    f2 pos_scene_normal; // Position of the cursor during previous frame
    window::InputEvent event;
    
    PointerClick2D(f2 pos_scene_normal, window::InputEvent event)
        :   pos_scene_normal    {pos_scene_normal},
            event               {event} {};
};

struct EventResultScene {
    enum Action {
        Grab,
        Release,
    } action = Release;

    EventResultScene() = default;
    EventResultScene(Action action): action {action} {};

    bool is_grab() { return action == Grab ? true : false;};
};



class Scene2D
{
    

    size_t id; // id from manager during creation
    size_t parent_id = 0; // If the scene is not a root scene it has a valid parent id


    f2 framebuffer_size_f;  // 
    Box2D window_box;       // The AABB of this scene as viewed from the window
    f2 cursor_pos_scene;    // updated during scene cursor update call
    f2 cursor_pos_normal;   // updated during scene cursor update call

    Camera2D camera;
    float zoom_factor = 1.2f;
    bool scene_grab = false; // Scene can be panned, usually with middle mouse button pressed
    bool quad_grab = false;


    RendererScene2D renderer2D; // each scene owns a renderer
    bool multisample_flag = false; // will render to multisampled framebuffer when set
    opengl::TextureFBMS framebuffer_multisample;
    opengl::TextureFB framebuffer;


public:
    SID sid;
    Str name;

    m3f3 frame_M_m_s; // dummy frame matrix for testing
    std::vector<ShapeS2D> points;
    std::vector<ShapeS2D> lines;
    std::vector<ShapeS2D> shapes;
    
    std::vector<size_t> quad_ids;

    std::vector<Graph2D<float>> graphs;

public:

    Scene2D(f2 _framebuffer_size);


    Str& get_name();
    void set_name(const Str& new_name);


    void   set_id(size_t id);
    size_t get_id() const ;
    void   set_parent_id(size_t id);
    size_t get_parent_id();


    bool is_multisampled();
    void enable_multisample();
    void disable_multisample();
    opengl::TextureFBMS& get_FBMS();
    opengl::TextureFB& get_FB();
    f2 get_framebuffer_size();
    void set_framebuffer_size(f2 size);


    Camera2D& get_camera();
    /** Set the width of the viewbox. Preserves aspect ratio. */
    void set_viewbox_width(float width);
    /** The factor with which a scroll-event multiplies the viewbox size on zoom out, or divides by during zoom-in events. */
    void set_zoom_factor(float new_zoom_factor);


    void set_cursor_pos(f2 cursor_pos_normalized);

    /** Does NOT update cursor position, as this is done when finding current target. */
    InputResponse handle_pointer_move(window::InputEvent& event);
    InputResponse handle_pointer_click(window::InputEvent& event);
    InputResponse handle_scroll(window::InputEvent& scroll_event);

    ShapeS2D& push_shape(Shape& shape);


    /** Highlights if a quad is located at currently set cursor position. */
    void try_hover_quad();
    /** Selects quad if located at currently set cursor position. */
    bool try_select_quad();
    /** Returns a found quad id and the quadbox in normalized viewbox coordinates. */
    QuadQuery try_find_quad_in_viewbox(f2 viewbox_pos_normalized);

    /** Add a quad subscene using an already manager-allocated scene.  */
    size_t add_subscene2D(size_t scene_id, f2 quad_pos);
    void push_quad_id(size_t quad_id);
    void push_quad(QuadS2D& quad);

private:

    void try_resize_hovered_quad(float size_factor);

    void release_grabs();

};



