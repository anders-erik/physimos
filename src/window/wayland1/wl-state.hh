
#pragma once

#include <stdio.h>
#include <sys/mman.h>

#include "wayland-client.h"
#include "wayland-cursor.h"

#include "lib/arr.hh"

#include "math/vecmat.hh"

#include "window/key.hh"
#include "io/input/user_input.hh"

#include "wl-defs.hh"
#include "wl-xdg.hh"
// #include "wl-seat.hh"
#include "wl-callbacks.hh"

struct WL_State;
// typedef struct WL_State;
// typedef struct client_state WaylandState;


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
    struct wl_shm *wl_shm;
    struct wl_shm_pool *pool;
    int shm_fd;
    int w;
    int h;
    bool allocated = false;
    bool buffer_busy = false;

    void init(WL_State* _state);
    void resize(i2 dims);
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

    void destroy()
    {
        const int size = w*h*4;

        munmap(data, size);
        wl_buffer_destroy(buffer);
        wl_shm_pool_destroy(pool);
        close(shm_fd);

        allocated = false;
    }
};


/** Wayland core objects */
struct WL_Core
{
    struct wl_display *display;
    struct wl_registry *registry;
    const struct wl_registry_listener registry_listener = {
        .global        = registry_global,
        .global_remove = registry_global_remove,
    };
    struct wl_compositor *compositor;

    struct wl_surface *surface_window;

    struct wl_surface *surface_content;
    struct wl_subcompositor *subcompositor;
    struct wl_subsurface *subsurface;
};

/** XDG objects */
struct WL_XDG
{
    struct xdg_wm_base *wm_base;
    struct xdg_surface *surface;
    struct xdg_toplevel *toplevel;
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

    // struct wl_surface *cursor_surface;
    // struct wl_cursor_image *cursor_image;

    Arr<UserInput> w_events;

    KeyModifiers key_modifiers;

    // Pointer state
    d2 pointer_pos_raw = {0.0, 0.0};
    d2 pointer_pos_last_sane = {0.0, 0.0};
    // d2 pointer_sane = {0.0, 0.0};
};

/* Wayland code */
struct WL_State {
    /* Wayland Objects */
    WL_Core core;
    /* XDG Objects */
    WL_XDG xdg;
    /* Buffer objects */
    WL_FrameBuffer fb;
    /* Subframebuffer for the content subsurface */
    WL_FrameBuffer subfb;
    /* Object to handle the cursor image */
    WL_Cursor cursor;

    /* Input*/
    WL_Input input;
    d2 raw_pointer;
    d2 sane_pointer;

    int running = 1;
};