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


struct WL_Surfaces
{
    int window_border_width = 5;
    int window_topbar_height = 20;

    i2 content_dims = {0, 0};

    bool window_size_change_detected = false;
    i2 size_change_dims = {0, 0};

    bool is_initialized = false;

    /* Pool to contain all surface buffers */
    WL_Pool pool;

    /* window surface*/
    WL_Surface window;
    /* content surface*/
    WL_Surface content;
    /* test surface*/
    WL_Surface test;
    /* EGL surface*/
    WL_Surface egl;


    void init(i2 _content_dims, Wayland* state);
    i2  content_to_window_dims(i2 _content_dims);
    i2  window_to_content_dims(i2 _window_dims);
    void full_reload_from_window_size(i2 new_window_dims);
    // void reload_with_new_window_dims(i2 new_window_dims);
    void set_content_dims(i2 _content_dims);
    void new_wl_buffers(Wayland* wayland);
    int get_all_surface_buffer_sizes_in_bytes();
    int get_offset(WL_Surface::Type _surface_type);
    void* get_content_pointer();
    void commit();
};
