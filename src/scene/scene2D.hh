
#pragma once

#include <vector>

#include "lib/pair.hh"

#include "window/auxevent.hh"

#include "opengl/program.hh"
#include "opengl/texture.hh"
#include "opengl/texture_fb.hh"
#include "opengl/texture_framebuffer_multi.hh"

#include "math/vecmat.hh"
#include "math/shape.hh"

#include "scene/render/renderer2D.hh"
#include "scene/shapeS2D.hh"
#include "scene/camera2D.hh"
#include "scene/quad_manager.hh"


struct GLFWWindow;


namespace scene {

struct QuadS2D;


/** Cursor movement event in normalized viewport coordinated. Usually recieved from owner of scene.
    [0,1]x[0,1] [unitless]
 */
struct PointerMovement2D 
{
    f2 pos_norm_prev; // Position of the cursor during previous frame
    f2 pos_norm_curr; // Current cursor position
    window::InputEvent event;
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

    Str name;

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
    opengl::TextureFrameBufferMultisample framebuffer_multisample;
    opengl::TextureFB framebuffer;

    m3f3 frame_M_m_s; // dummy frame matrix for testing


    std::vector<ShapeS2D> points;
    std::vector<ShapeS2D> lines;
    std::vector<ShapeS2D> shapes;

    void release_grabs();
    void try_resize_hovered_quad(float size_factor);


public:
    // QuadManager quad_manager;
    std::vector<size_t> quad_ids;

public:

    Scene2D(f2 _framebuffer_size);


    Str& get_name();
    void set_name(const Str& new_name);

    void clear_cursor_grab();

    Box2D get_window_box();


    void   set_id(size_t id);
    size_t get_id() const ;
    void   set_parent_id(size_t id);
    size_t get_parent_id();

    bool is_multisampled();
    void enable_multisample();
    void disable_multisample();

    f2 get_framebuffer_size();
    void set_framebuffer_size(f2 size);

    Box2D get_camera_viewbox();
    /** Set the width of the viewbox. Preserves aspect ratio. */
    void set_viewbox_width(float width);
    /** The factor with which a scroll-event multiplies the viewbox size on zoom out, or divides by during zoom-in events. */
    void set_zoom_factor(float new_zoom_factor);

    void set_cursor_pos(f2 cursor_pos_normalized);
    void set_window_box(Box2D new_window_box);

    // Check if position is within the bounding box of a quad in current scene
    QuadS2D* try_match_cursor_to_quad(f2 pos_scene);

    /** Highlights if a quad is located at currently set cursor position. */
    void try_hover_quad();
    /** Selects quad if located at currently set cursor position. */
    bool try_select_quad();

    /** Loop through quads and identify the ones showing a scene */
    std::vector<size_t> get_subscene_ids();

    /** Returns a found subscene id and the quadbox in normalized scene coordinates. */
    Pair<size_t, Box2D> try_find_subscene(f2 viewbox_pos_normalized);

    /** Does NOT update cursor position, as this is done when finding current traget at the beginning of each frame. */
    EventResultScene handle_pointer_move(PointerMovement2D cursor_event);
    EventResultScene handle_pointer_click(PointerClick2D pointer_click);
    EventResultScene handle_scroll(window::InputEvent scroll_event);

    void push_quad_id(size_t quad_id);
    void push_quad(scene::QuadS2D& quad);
    ShapeS2D& push_shape(Shape& shape);

    
    /** Add a quad subscene using an already manager-allocated scene.  */
    size_t add_subscene2D(size_t scene_id, f2 quad_pos);



    void update();
    /** Recursively render all subscenes */
    void render_subscene_textures();

    void render_to_window();
    unsigned int render_to_texture();

private:
    /** main render call */
    void render();
};



}