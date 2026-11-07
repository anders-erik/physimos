#pragma once


#include "lib/arr.hh"
#include "lib/llist.hh"
#include "lib/pixel_buffer.hh"

#include "io/input/user_input.hh"

#include "wl-defs.hh"
#include "wl-callbacks.hh"

#include "wl-egl.hh"


struct WL_Cursor
{
    struct wl_cursor        *cursor;
    struct wl_surface       *surface;
    struct wl_cursor_image  *image;
    struct wl_cursor_theme  *theme;
    struct wl_buffer        *buffer;

    void create_default(wl_compositor* _compositor, wl_shm* _shm_surface_window)
    {
        // CREATE CURSOR
        theme = wl_cursor_theme_load(NULL, 24, _shm_surface_window);
        cursor = wl_cursor_theme_get_cursor(theme, "left_ptr");
        image = cursor->images[0];
        buffer = wl_cursor_image_get_buffer(image);

        surface = wl_compositor_create_surface(_compositor);
        wl_surface_attach(surface, buffer, 0, 0);
        wl_surface_commit(surface);
    }

};



/** Wayland core objects */
struct WL_Core
{
    struct wl_display *display;
    const struct wl_display_listener display_listener = {
        .error      = display_error,
        .delete_id  = display_delete_id,
    };
    struct wl_registry *registry;
    const struct wl_registry_listener registry_listener = {
        .global        = registry_global,
        .global_remove = registry_global_remove,
    };
    struct wl_compositor *compositor;
    struct wl_subcompositor *subcompositor;
};

/** XDG objects */
struct WL_XDG
{
    struct xdg_wm_base *wm_base;
    const struct xdg_wm_base_listener xdg_wm_base_listener = {
        .ping = xdg_wm_base_ping,
    };
    struct xdg_surface *surface;
    const struct xdg_surface_listener xdg_surface_listener = {
        .configure = xdg_surface_configure,
    };
    struct xdg_toplevel *toplevel;
    const struct xdg_toplevel_listener xdg_toplevel_listener = {
        .configure = xdg_toplevel_configure,
        .close = xdg_toplevel_close,
        .configure_bounds = xdg_toplevel_configure_bounds,
        .wm_capabilities = xdg_toplevel_wm_capabilities,
    };
};

struct WL_Input
{
    struct wl_seat *seat;
    const struct wl_seat_listener seat_listener = {
        .capabilities = seat_capabilities,
        .name = NULL,
    };

    struct wl_pointer* pointer = NULL;
    const struct wl_pointer_listener pointer_listener = {
        .enter  = pointer_enter,
        .leave  = pointer_leave,
        .motion = pointer_motion,
        .button = pointer_button,
        .axis   = pointer_axis,
    };
    struct wl_keyboard* keyboard = NULL;
    const struct wl_keyboard_listener keyboard_listener = {
        .keymap      = keyboard_keymap,
        .enter       = keyboard_enter,
        .leave       = keyboard_leave,
        .key         = keyboard_key,
        .modifiers   = keyboard_modifiers,
        .repeat_info = keyboard_repeat_info,
    };

    enum Target
    {
        Window,
        Content,
        Test,
        EGL,
        None,
    } target = None;

    // struct wl_surface *cursor_surface;
    // struct wl_cursor_image *cursor_image;

    Arr<UserInput> content_events;

    KeyModifiers key_modifiers;

    // Pointer state
    d2 pointer_pos_window_raw = {0.0, 0.0};
    d2 pointer_pos_window_sane = {0.0, 0.0};

    // d2 pointer_pos_content_raw = {0.0, 0.0};
    d2 pointer_pos_content_sane = {0.0, 0.0};
    // d2 pointer_sane = {0.0, 0.0};
};





struct WL_Surface
{
    i2 size = {0, 0};
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

    int get_size_pixels() { return size.x * size.y; };
    int get_size_bytes() { return size.x * size.y * 4; };

 

    void init(struct wl_compositor* compositor, struct wl_shm_pool* pool, int offset, WL_State* state)
    {
        surface = wl_compositor_create_surface(compositor);

        buffer = wl_shm_pool_create_buffer(pool, offset, size.x, size.y, size.x*4, wl_shm_format::WL_SHM_FORMAT_XRGB8888);
        wl_buffer_add_listener(buffer, &wl_buffer_listener, state); 
        // wl_surface_attach(surface, buffer, 0, 0);
    }

    void create_new_buffer(struct wl_shm_pool* pool, int offset, WL_State* state)
    {
        buffer = wl_shm_pool_create_buffer(pool, offset, size.x, size.y, size.x*4, wl_shm_format::WL_SHM_FORMAT_XRGB8888);
        wl_buffer_add_listener(buffer, &wl_buffer_listener, state);
        // wl_surface_attach(surface, buffer, 0, 0);
    }

    void create_subsurface(struct wl_subcompositor* subcompositor, struct wl_surface* parent)
    {
        subsurface = wl_subcompositor_get_subsurface(subcompositor, surface, parent);

        if (!subsurface)
        {
            fprintf(stderr, "Failed to create subsurface.\n");
            return;
        }

        wl_subsurface_set_position(subsurface, pos.x, pos.y);
    }

    void clear(PX32 color)
    {
        for(uint i = 0; i < get_size_bytes(); i++)
        {
            (pixel_buffer.buf)[i] = color;
        }
    }

    void commit()
    {
        wl_surface_attach(surface, buffer, 0, 0);
        wl_surface_damage_buffer(surface, 0, 0, size.x, size.y);
        wl_surface_commit(surface);
        surface_is_busy = true;
    }
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

    bool size_change_detected = false;
    i2 size_change_dims = {0, 0};

    bool is_initialized = false;

    /* Pool to contain all surface buffers */
    WL_Pool pool;

    struct OldBuffer {
        WL_Surface::Type type;
        struct wl_buffer* buffer;
    };
    Arr<OldBuffer> old_buffers;
    // LList<OldBuffer> old_buffers;

    /* window surface*/
    WL_Surface window;
    /* content surface*/
    WL_Surface content;
    /* test surface*/
    WL_Surface test;
    /* EGL surface*/
    WL_Surface egl;



    void init(i2 _content_dims, WL_State* state);

    i2  content_to_window_dims(i2 _content_dims)
    {
        return  {   content_dims.x + window_border_width * 2, 
                    content_dims.y + window_topbar_height + window_border_width };
    }
    i2  window_to_content_dims(i2 _window_dims)
    {
        return  {   _window_dims.x - window_border_width * 2, 
                    _window_dims.y - window_topbar_height - window_border_width };
    }

    uint calculate_total_buffer_size_bytes(i2 _content_dims)
    {
        return 0;
    }

    void set_content_dims(i2 _content_dims);

    void new_wl_buffers(WL_State* state);

    int get_all_surface_buffer_sizes_in_bytes()
    {
        return (window.get_size_bytes() + content.get_size_bytes() + test.get_size_bytes() + egl.get_size_bytes());
    }

    int get_offset(WL_Surface::Type _surface_type)
    {
        switch (_surface_type)
        {
        case WL_Surface::Window:
            return 0;
            break;
        case WL_Surface::Content:
            return get_offset(WL_Surface::Window) + window.get_size_bytes();
            break;
        case WL_Surface::Test:
            return get_offset(WL_Surface::Content) + content.get_size_bytes();
            break;
        
        default:
            break;
        }

        return 0;
    }

    void* get_content_pointer()
    {
        return (void*) ((char*)pool.data + window.get_size_bytes());
    }
    
    void commit();
};

/* Wayland code */
struct WL_State
{
    /* Wayland Objects */
    WL_Core core;
    /* XDG Objects */
    WL_XDG xdg;
    /* Surface handler*/
    WL_Surfaces surfaces;

    WL_EGL egl;
    /* Object to handle the cursor image */
    WL_Cursor cursor;
    /* Input*/
    WL_Input input;

    int running = 1;
};
