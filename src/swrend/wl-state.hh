
#pragma once

#include <stdio.h>

#include "wayland-client.h"

#include "lib/arr.hh"

#include "math/vecmat.hh"

#include "window/key.hh"
#include "swrend/event.hh"


struct WLFB
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

    void init();
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
};

/** Wyaland callbacks */
struct WLCB
{
    
};

/** Wyaland objects */
struct WLOBJ
{
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_surface *surface;
    struct wl_seat *seat;
};

/** Wyaland objects */
struct XDGOBJ
{
    struct xdg_wm_base *wm_base;
    struct xdg_surface *surface;
    struct xdg_toplevel *toplevel;
};

struct WLINPUT
{
    Arr<WEvent> w_events;

    // Pointer state
    d2 pointer_pos_raw = {0.0, 0.0};
    // d2 pointer_sane = {0.0, 0.0};
};

/* Wayland code */
typedef struct client_state {
    /* Wayland Objects */
    WLOBJ wl;
    /* XDG Objects */
    XDGOBJ xdg;
    /* Buffer objects */
    WLFB fb;
    /* Callbacks*/
    WLCB callbacks;

    /* Input*/
    WLINPUT input;
    d2 raw_pointer;
    d2 sane_pointer;

    int running = 1;
} client_state;