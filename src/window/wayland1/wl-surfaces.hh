#pragma once


#include "lib/arr.hh"
#include "lib/llist.hh"
#include "lib/pixel_buffer.hh"

#include "io/input/user_input.hh"

#include "wl-defs.hh"
#include "wl-callbacks.hh"

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








struct WL_Pool
{
    struct wl_shm *wl_shm = nullptr;
    struct wl_shm_pool *pool = nullptr;
    int shm_fd = -1;
    int size = 0;
    void* data = nullptr;

    void randname(char *buf);
    int create_shm_file();
    int allocate_shm_file(int size);

    void init(int _size);
};


struct WL_Decoration
{
    /* window/decoration surface*/
    WL_Surface surface;

    int border_width = 15;
    int topbar_height = 30;
    bool maximized = false;

    WLDecorationArea area                   = WLDecorationArea::None;
    WLDecorationArea area_previous_frame    = WLDecorationArea::None;

    



    /* y-zero is at top of buffer */
    WLDecorationArea get_area(i2 pos_raw)
    {
        WLDecorationArea area;

        // Step through the cases from y = 0 to y = y_max

        if(pos_raw.y < border_width)
        {
            if(pos_raw.x < border_width)
                area = WLDecorationArea::EdgeTopLeft;
            else if(pos_raw.x > surface.pixel_buffer.w - border_width)
                area = WLDecorationArea::EdgeTopRight;
            else
                area = WLDecorationArea::EdgeTop;
        }
        else if(pos_raw.y < topbar_height + border_width )
        {
            if(pos_raw.x < border_width)
                area = WLDecorationArea::EdgeLeft;
            else if(pos_raw.x < topbar_height + border_width)
                area = WLDecorationArea::TopLeft;
            else if(pos_raw.x < (surface.pixel_buffer.w - border_width - (topbar_height * 3)))
                area = WLDecorationArea::Topbar;
            else if(pos_raw.x < (surface.pixel_buffer.w - border_width - (topbar_height * 2)))
                area = WLDecorationArea::MinimizeButton;
            else if(pos_raw.x < (surface.pixel_buffer.w - border_width - (topbar_height * 1)))
                area = WLDecorationArea::MaximizeButton;
            else if(pos_raw.x < (surface.pixel_buffer.w - border_width))
                area = WLDecorationArea::CloseButton;
            else
                area = WLDecorationArea::EdgeRight;
        }
        else if(pos_raw.y < (surface.pixel_buffer.h - border_width))
        {
            if(pos_raw.x < border_width)
                area = WLDecorationArea::EdgeLeft;
            else if(pos_raw.x > (surface.pixel_buffer.w - border_width))
                area = WLDecorationArea::EdgeRight;
            else
                area = WLDecorationArea::None;
        }
        else if (pos_raw.y < surface.pixel_buffer.h)
        {
            if(pos_raw.x < border_width)
                area = WLDecorationArea::EdgeBottomLeft;
            else if(pos_raw.x < (surface.pixel_buffer.w - border_width))
                area = WLDecorationArea::EdgeBottom;
            else if(pos_raw.x < surface.pixel_buffer.w)
                area = WLDecorationArea::EdgeBottomRight;
            else
                area = WLDecorationArea::None;
        }
        else
        {
            area = WLDecorationArea::None;
        }

        return area;
    }


    void draw();
};

struct WL_Surfaces
{
    bool window_size_change_detected = false;
    i2 size_change_dims = {0, 0};

    /* Pool to contain all surface buffers */
    WL_Pool pool;

    WL_Decoration decorations;

    /* content surface*/
    WL_Surface content;
    /* test surface*/
    WL_Surface test;
    /* EGL surface*/
    WL_Surface egl;


    void init(i2 _content_dims, Wayland* state);
    void full_reload_from_window_size(i2 new_window_dims, Wayland* wayland);

    i2  content_to_window_dims(i2 content_dims);
    i2  window_to_content_dims(i2 window_dims);
    void set_surface_dims(i2 window_dims);
    int get_all_surface_buffer_sizes_in_bytes();
    int get_offset(WL_Surface::Type _surface_type);
    void* get_buffer_pointer(WL_Surface::Type surface_type);

    void commit();
};
