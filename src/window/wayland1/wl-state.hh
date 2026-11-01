#pragma once


#include "lib/arr.hh"
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

struct WL_FrameBuffer
{
    enum Format
    {
        RGB,
        RGBA
    } format = RGBA;

    wl_shm_format shm_format;

    uint32_t *data = 0; // pointer to the mmaped framebuffer
    struct wl_buffer *buffer = 0;
    const struct wl_buffer_listener wl_buffer_listener = {
        .release = wl_buffer_release,
    };
    struct wl_shm *wl_shm;
    struct wl_shm_pool *pool;
    int shm_fd;

    int w;
    int h;
    bool allocated = false;
    bool buffer_busy = false;

    void randname(char *buf);
    int create_shm_file();
    int allocate_shm_file(size_t size);

    void init(WL_State* _state);
    void resize(i2 dims);
    void destroy();




    void* ptr() {return (void*) data;}

    void clear_gray()
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                    data[y * w + x] = 0xFF999999;
            }
        }
    }

    void clear_green()
    {
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                    data[y * w + x] = 0xFF55AA55;
            }
        }
    }

    
};


struct WL_Surface
{
    i2 pos = {400, 200};
    i2 size = {50, 50};
    WL_FrameBuffer fb;
    struct wl_surface* surface = nullptr;
    struct wl_subsurface *subsurface = nullptr;


    void init(struct wl_compositor* compositor, WL_State* state)
    {
        surface = wl_compositor_create_surface(compositor);

        fb.w = size.x;
        fb.h = size.y;
        fb.init(state);
        fb.clear_gray(); 
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

    void resize(i2 new_dims, struct wl_compositor* compositor)
    {
        fb.destroy();
        wl_surface_destroy(surface);

        surface = wl_compositor_create_surface(compositor);

        size = new_dims;

        fb.w = size.x;
        fb.h = size.y;
        fb.resize(new_dims);
        fb.clear_gray(); 
    }

    void commit()
    {
        wl_surface_damage_buffer(surface, 0, 0, fb.w, fb.h);
        wl_surface_attach(surface, fb.buffer, 0, 0);
        wl_surface_commit(surface);
        fb.buffer_busy = true;
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

/* Wayland code */
struct WL_State
{
    /* Wayland Objects */
    WL_Core core;
    /* XDG Objects */
    WL_XDG xdg;
    /* window surface*/
    WL_Surface surface_window;
    /* content surface*/
    WL_Surface surface_content;
    /* test surface*/
    WL_Surface surface_test;
    /* EGL surface*/
    WL_Surface surface_egl;
    WL_EGL egl;
    // /* Buffer objects */
    // WL_FrameBuffer fb;
    /* Object to handle the cursor image */
    WL_Cursor cursor;
    /* Input*/
    WL_Input input;

    int window_border_width = 5;
    int window_topbar_height = 20;

    int running = 1;
    bool size_changed = false;
    i2 new_dims;
};