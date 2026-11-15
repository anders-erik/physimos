#pragma once


#include "lib/arr.hh"
#include "lib/llist.hh"
#include "lib/pixel_buffer.hh"

#include "io/input/user_input.hh"

#include "wl-defs.hh"
#include "wl-callbacks.hh"
#include "wl-pool.hh"

#include "wl-egl.hh"


struct WL_Surface
{
    // i2 size = {0, 0};
    PixelBuffer pixel_buffer;
    struct wl_buffer *buffer = nullptr;
    wl_shm_format format = wl_shm_format::WL_SHM_FORMAT_XRGB8888;
    const struct wl_buffer_listener wl_buffer_listener = {
        .release = wl_buffer_release,
    };

    struct wl_surface* surface = nullptr;
    bool surface_is_busy = false;

    struct wl_subsurface *subsurface = nullptr;
    i2 pos = {0, 0};


    enum Type
    {
        Window,
        Content,
        Test,
        EGL,
        None,
    } type = None;

    void init(struct wl_compositor* compositor, struct wl_shm_pool* pool, int offset, Wayland* state);
    void create_new_buffer(struct wl_shm_pool* pool, int offset, Wayland* state);
    void create_subsurface(struct wl_subcompositor* subcompositor, struct wl_surface* parent);
    
    void clear(PX32 color);
    
    int get_size_pixels();
    int get_size_bytes();

    void commit();
};




struct WL_Decoration
{
    /* window/decoration surface*/
    WL_Surface surface;

    WL_Pool pool;

    int border_width = 15;
    int topbar_height = 30;
    bool maximized = false;

    WLDecorationArea area                   = WLDecorationArea::None;
    WLDecorationArea area_previous_frame    = WLDecorationArea::None;

    void init(wl_compositor * compositor, i2 window_dims, i2 _monitor_dims, Wayland* wayland);
    void full_reload_from_window_size(i2 window_dims, Wayland* wayland);

    /* y-zero is at top of buffer */
    WLDecorationArea get_area(i2 pos_raw);
    /* Derived position based on the values of topbar height & border width */
    i2 get_content_pos();
    /* Draw decorations to memory buffer using software renderer */
    void draw();
};


struct WL_OpenGL
{
    /* Wayland surface*/
    WL_Surface surface;
    /* EGL object */
    WL_EGL egl;


    void init(struct wl_compositor* compositor, i2 _content_dims, i2 _surface_pos, Wayland* state);
    void full_reload_from_content_size(i2 new_content_dims, i2 content_pos, Wayland* wayland);
};


struct WL_SoftwareRend
{
    /* Wayland surface*/
    WL_Surface surface;

    WL_Pool pool;

    void init(struct wl_compositor* compositor, i2 _content_dims, i2 _monitor_dims, i2 _surface_pos, Wayland* state);
    void full_reload_from_content_size(i2 new_content_dims, i2 content_pos, Wayland* wayland);
};

struct WL_Surfaces
{
    bool window_size_change_detected = false;
    i2 size_change_dims = {0, 0};

    WaylandRenderingType render_type = WaylandRenderingType::Buffer;

    /* Wayland client decorations */
    WL_Decoration decorations;
    /* Software render context */
    WL_SoftwareRend swr;
    /* OpenGL context using EGL */
    WL_OpenGL opengl;


    void init(i2 _content_dims, i2 _monitor_dims, WaylandRenderingType _rendering_type, Wayland* state);
    void full_reload_from_window_size(i2 new_window_dims, Wayland* wayland);

    i2  content_to_window_dims(i2 content_dims);
    i2  window_to_content_dims(i2 window_dims);
    int get_all_surface_buffer_sizes_in_bytes();
    int get_offset(WL_Surface::Type _surface_type);
    void* get_buffer_pointer(WL_Surface::Type surface_type);
    i2 get_content_dims();

    void commit();
};
